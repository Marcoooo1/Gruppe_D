#ifndef TCP_CONNECT_H
#define TCP_CONNECT_H

#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

// Winsock-Hilfsfunktionen
bool initWinsock();
void cleanupWinsock();

// ---------------------------
// JSON-Stream-Reader-Basis
// ---------------------------
class JsonStreamReader {
protected:
    std::string ip;
    int port;
    std::string outputFile;
    int maxMessages; // Anzahl der JSON-Nachrichten, die gespeichert werden sollen

public:
    JsonStreamReader(const std::string& ip, int port,
                     const std::string& outFile,
                     int maxMessages = 1);

    virtual ~JsonStreamReader();

    // Objekt direkt in std::thread verwendbar
    void operator()();

protected:
    virtual void run();
    SOCKET createAndConnectSocket();
};

// Spezifische Klassen für Laser & Odom
class LaserReader : public JsonStreamReader {
public:
    explicit LaserReader(const std::string& ip);
};

class OdomReader : public JsonStreamReader {
public:
    explicit OdomReader(const std::string& ip);
};

// ---------------------------
// NEU: CommandClient für Port 9999
// ---------------------------
class CommandClient {
private:
    std::string ip;
    int port;
    SOCKET sock;
    bool connected;

public:
    explicit CommandClient(const std::string& ip, int port = 9999);
    ~CommandClient();

    // Einmalige Verbindung herstellen
    bool connectToServer();

    // Command-String an Turtlebot schicken
    bool sendCommand(const std::string& cmd);

    // Verbindung explizit schließen (optional, wird im Destruktor auch gemacht)
    void close();
};

#endif // TCP_CONNECT_H
