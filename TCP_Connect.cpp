#include "TCP_Connect.h"
#include <iostream>
#include <fstream>

using namespace std;

// ------------------------
// Winsock Hilfsfunktionen
// ------------------------

bool initWinsock()
{
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        cerr << "WSAStartup failed: " << result << endl;
        return false;
    }
    return true;
}

void cleanupWinsock()
{
    WSACleanup();
}

// ------------------------
// JsonStreamReader
// ------------------------

JsonStreamReader::JsonStreamReader(const string& ip,
                                   int port,
                                   const string& outFile,
                                   int maxMessages)
    : ip(ip),
      port(port),
      outputFile(outFile),
      maxMessages(maxMessages)
{
}

JsonStreamReader::~JsonStreamReader()
{
}

void JsonStreamReader::operator()()
{
    run();
}

SOCKET JsonStreamReader::createAndConnectSocket()
{
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        cerr << "[Port " << port << "] Socket creation failed\n";
        return INVALID_SOCKET;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());

    cout << "[Port " << port << "] Verbinde..." << endl;

    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        cerr << "[Port " << port << "] Connect failed\n";
        closesocket(sock);
        return INVALID_SOCKET;
    }

    cout << "[Port " << port << "] Verbunden! Sammle JSON..." << endl;
    return sock;
}

void JsonStreamReader::run()
{
    SOCKET sock = createAndConnectSocket();
    if (sock == INVALID_SOCKET) {
        return;
    }

    ofstream jsonFile(outputFile);
    if (!jsonFile) {
        cerr << "[Port " << port << "] Konnte Datei nicht öffnen: "
             << outputFile << endl;
        closesocket(sock);
        return;
    }

    jsonFile << "[\n";

    const string START_MARKER = "---START---";
    const string END_MARKER   = "___END___";

    string bufferAccum;
    char recvBuf[4096];
    bool first = true;
    int msgCount = 0;

    while (msgCount < maxMessages) {
        int len = recv(sock, recvBuf, sizeof(recvBuf) - 1, 0);
        if (len > 0) {
            recvBuf[len] = '\0';
            bufferAccum += recvBuf;

            while (true) {
                size_t startPos = bufferAccum.find(START_MARKER);
                if (startPos == string::npos)
                    break;

                size_t endPos = bufferAccum.find(END_MARKER, startPos);
                if (endPos == string::npos)
                    break;

                size_t jsonStart = startPos + START_MARKER.size();
                size_t jsonLen   = endPos - jsonStart;

                string jsonSnippet = bufferAccum.substr(jsonStart, jsonLen);

                bufferAccum.erase(0, endPos + END_MARKER.size());

                // Whitespace vorne entfernen
                while (!jsonSnippet.empty() &&
                       (jsonSnippet.front() == '\n' ||
                        jsonSnippet.front() == '\r' ||
                        jsonSnippet.front() == ' '))
                {
                    jsonSnippet.erase(jsonSnippet.begin());
                }

                // Whitespace hinten entfernen
                while (!jsonSnippet.empty() &&
                       (jsonSnippet.back() == '\n' ||
                        jsonSnippet.back() == '\r' ||
                        jsonSnippet.back() == ' '))
                {
                    jsonSnippet.pop_back();
                }

                if (!first)
                    jsonFile << ",\n";
                first = false;

                jsonFile << jsonSnippet;

                msgCount++;
                cout << "[Port " << port << "] Message "
                     << msgCount << " gespeichert." << endl;

                if (msgCount >= maxMessages)
                    break;
            }
        }
        else if (len == 0) {
            cout << "[Port " << port << "] Verbindung geschlossen." << endl;
            break;
        }
        else {
            cerr << "[Port " << port << "] recv Fehler" << endl;
            break;
        }
    }

    jsonFile << "\n]\n";
    jsonFile.close();

    closesocket(sock);
    cout << "[Port " << port << "] Fertig! JSON gespeichert in "
         << outputFile << endl;
}

// ------------------------
// LaserReader
// ------------------------

LaserReader::LaserReader(const string& ip)
    : JsonStreamReader(ip, 9997, "laser_output.json", 1)
{
}

// ------------------------
// OdomReader
// ------------------------

OdomReader::OdomReader(const string& ip)
    : JsonStreamReader(ip, 9998, "odom_output.json", 1)
{
}

// ------------------------
// CommandClient (Port 9999)
// ------------------------

CommandClient::CommandClient(const string& ip, int port)
    : ip(ip), port(port), sock(INVALID_SOCKET), connected(false)
{
}

CommandClient::~CommandClient()
{
    close();
}

bool CommandClient::connectToServer()
{
    if (connected) {
        return true; // schon verbunden
    }

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        cerr << "[Port " << port << "] Socket creation failed\n";
        return false;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());

    cout << "[Port " << port << "] Verbinde (CommandClient)..." << endl;

    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        cerr << "[Port " << port << "] Connect failed (CommandClient)\n";
        closesocket(sock);
        sock = INVALID_SOCKET;
        return false;
    }

    cout << "[Port " << port << "] Verbunden (CommandClient)!" << endl;
    connected = true;
    return true;
}

bool CommandClient::sendCommand(const string& cmd)
{
    if (!connected) {
        if (!connectToServer()) {
            return false;
        }
    }

    int len = send(sock, cmd.c_str(), (int)cmd.size(), 0);
    if (len == SOCKET_ERROR) {
        cerr << "[Port " << port << "] send Fehler\n";
        close();
        return false;
    }

    cout << "[Port " << port << "] Command gesendet: " << cmd << endl;
    return true;
}

void CommandClient::close()
{
    if (connected && sock != INVALID_SOCKET) {
        closesocket(sock);
        sock = INVALID_SOCKET;
        connected = false;
        cout << "[Port " << port << "] Verbindung (CommandClient) geschlossen.\n";
    }
}
