// verbindung zu beiden ports und speichern in laser_output.json und odom_output.json
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <mutex>

#pragma comment(lib, "ws2_32.lib")

class JsonStreamReader {
protected:
    std::string ip;
    int port;
    std::string outputFile;

public:
    JsonStreamReader(const std::string& ip, int port, const std::string& out)
        : ip(ip), port(port), outputFile(out) {}

    virtual ~JsonStreamReader() {}

    void operator()() {
        run();
    }

protected:
    virtual void run() {
        SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock == INVALID_SOCKET) {
            std::cerr << "[Port " << port << "] Socket creation failed\n";
            return;
        }

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.c_str());

        std::cout << "[Port " << port << "] Verbinde...\n";

        if (connect(sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
            std::cerr << "[Port " << port << "] Connect failed\n";
            closesocket(sock);
            return;
        }

        std::cout << "[Port " << port << "] Verbunden! Sammle JSON...\n";

        std::ofstream jsonFile(outputFile);
        if (!jsonFile) {
            std::cerr << "[Port " << port << "] Konnte Datei nicht öffnen.\n";
            closesocket(sock);
            return;
        }

        jsonFile << "[\n";

        const std::string START_MARKER = "---START---";
        const std::string END_MARKER = "___END___";

        std::string bufferAccum;
        char recvBuf[4096];
        bool first = true;

        int maxMessages = 1;
        int msgCount = 0;

        while (msgCount < maxMessages) {
            int len = recv(sock, recvBuf, sizeof(recvBuf) - 1, 0);
            if (len > 0) {
                recvBuf[len] = '\0';
                bufferAccum += recvBuf;

                while (true) {
                    std::size_t startPos = bufferAccum.find(START_MARKER);
                    if (startPos == std::string::npos)
                        break;

                    std::size_t endPos = bufferAccum.find(END_MARKER, startPos);
                    if (endPos == std::string::npos)
                        break;

                    std::size_t jsonStart = startPos + START_MARKER.size();
                    std::size_t jsonLen = endPos - jsonStart;

                    std::string jsonSnippet =
                        bufferAccum.substr(jsonStart, jsonLen);

                    bufferAccum.erase(0, endPos + END_MARKER.size());

                    while (!jsonSnippet.empty() &&
                        (jsonSnippet.front() == '\n' || jsonSnippet.front() == '\r' || jsonSnippet.front() == ' '))
                        jsonSnippet.erase(jsonSnippet.begin());

                    while (!jsonSnippet.empty() &&
                        (jsonSnippet.back() == '\n' || jsonSnippet.back() == '\r' || jsonSnippet.back() == ' '))
                        jsonSnippet.pop_back();

                    if (!first)
                        jsonFile << ",\n";
                    first = false;

                    jsonFile << jsonSnippet;

                    msgCount++;
                    std::cout << "[Port " << port << "] Message "
                              << msgCount << " gespeichert.\n";

                    if (msgCount >= maxMessages)
                        break;
                }
            }
            else if (len == 0) {
                std::cout << "[Port " << port << "] Verbindung geschlossen.\n";
                break;
            }
            else {
                std::cerr << "[Port " << port << "] recv Fehler\n";
                break;
            }
        }

        jsonFile << "\n]\n";
        jsonFile.close();

        closesocket(sock);
        std::cout << "[Port " << port << "] Fertig! JSON gespeichert.\n";
    }
};


// -----------------------------------------------------------
// Spezifische Klassen für Laser & Odom (nur Port + Dateiname)
// -----------------------------------------------------------

class LaserReader : public JsonStreamReader {
public:
    LaserReader(const std::string& ip)
        : JsonStreamReader(ip, 9997, "laser_output.json") {}
};

class OdomReader : public JsonStreamReader {
public:
    OdomReader(const std::string& ip)
        : JsonStreamReader(ip, 9998, "odom_output.json") {}
};


// -----------------------------------------------------------
// Main – startet beide Reader parallel
// -----------------------------------------------------------

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    const std::string ip = "192.168.100.52";

    LaserReader laser(ip);
    OdomReader odom(ip);

    std::thread tLaser(std::ref(laser));
    std::thread tOdom(std::ref(odom));

    tLaser.join();
    tOdom.join();

    WSACleanup();

    std::cout << "Alle Verbindungen beendet.\n";
    return 0;
}
