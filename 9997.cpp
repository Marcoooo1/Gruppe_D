#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <fstream>
#include <string>

// Linker: ws2_32 (hast du in Code::Blocks ja schon eingetragen)

int main() {
    // Winsock initialisieren
    WSADATA wsaData;
    int wsaResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaResult != 0) {
        std::cerr << "WSAStartup failed: " << wsaResult << std::endl;
        return 1;
    }

    // Socket erstellen
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed" << std::endl;
        WSACleanup();
        return 1;
    }

    // TurtleBot IP + Port
    const char* ip = "192.168.100.52";   // ggf. anpassen
    int port = 9997;

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    if (addr.sin_addr.s_addr == INADDR_NONE) {
        std::cerr << "Invalid IP address format" << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::cout << "Verbinde zu " << ip << ":" << port << " ..." << std::endl;

    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        std::cerr << "Connect failed" << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::cout << "Verbunden! Sammle komplette Messages zwischen ---START--- und ___END___.\n";

    // Output-JSON öffnen
    std::ofstream jsonFile("laser_output.json");
    if (!jsonFile) {
        std::cerr << "Konnte laser_output.json nicht öffnen" << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Wir schreiben ein JSON-Array
    jsonFile << "[\n";

    const std::string START_MARKER = "---START---";
    const std::string END_MARKER   = "___END___";

    std::string bufferAccum;        // hier sammeln wir den Stream
    char recvBuf[4096];

    int maxMessages = 50;
    int msgCount = 0;
    bool first = true;

    while (msgCount < maxMessages) {
        int len = recv(sock, recvBuf, sizeof(recvBuf) - 1, 0);
        if (len > 0) {
            recvBuf[len] = '\0';
            bufferAccum += recvBuf;

            // Solange wir mindestens ein komplettes Paket im Puffer haben:
            while (true) {
                std::size_t startPos = bufferAccum.find(START_MARKER);
                if (startPos == std::string::npos)
                    break;

                std::size_t endPos = bufferAccum.find(END_MARKER, startPos);
                if (endPos == std::string::npos)
                    break; // noch nicht komplett angekommen

                // JSON-Teil OHNE Marker herausschneiden
                std::size_t jsonStart = startPos + START_MARKER.size();
                std::size_t jsonLen   = endPos - jsonStart;
                std::string jsonSnippet = bufferAccum.substr(jsonStart, jsonLen);

                // Für die nächste Suche alles bis inklusive END_MARKER entfernen
                std::size_t removeLen = endPos + END_MARKER.size();
                bufferAccum.erase(0, removeLen);

                // Whitespace trimmen (einfach grob vorn/hinten)
                while (!jsonSnippet.empty() && (jsonSnippet.front() == '\n' || jsonSnippet.front() == '\r' || jsonSnippet.front() == ' ')) {
                    jsonSnippet.erase(jsonSnippet.begin());
                }
                while (!jsonSnippet.empty() && (jsonSnippet.back() == '\n' || jsonSnippet.back() == '\r' || jsonSnippet.back() == ' ')) {
                    jsonSnippet.pop_back();
                }

                // In Datei schreiben
                if (!first) {
                    jsonFile << ",\n";
                }
                first = false;

                jsonFile << jsonSnippet;   // das ist bereits ein gültiges JSON-Objekt

                std::cout << "Message " << msgCount << " gespeichert.\n";
                msgCount++;

                if (msgCount >= maxMessages) break;
            }
        }
        else if (len == 0) {
            std::cout << "Verbindung geschlossen." << std::endl;
            break;
        }
        else {
            std::cerr << "recv Fehler" << std::endl;
            break;
        }
    }

    jsonFile << "\n]\n";
    jsonFile.close();

    closesocket(sock);
    WSACleanup();

    std::cout << "Fertig! " << msgCount << " komplette Messages in 'laser_output.json' gespeichert.\n";
    return 0;
}
