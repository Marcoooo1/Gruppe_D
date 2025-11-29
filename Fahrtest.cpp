#include <iostream>
#include <thread>
#include <chrono>

#include "CMD_Parser.h"    // dein buildCmdString(...)
#include "TCP_Connect.h"   // dein CommandClient, initWinsock(), cleanupWinsock()

using namespace std;

int main()
{
    // 1. Winsock initialisieren
    if (!initWinsock()) {
        cerr << "Winsock Init fehlgeschlagen\n";
        return 1;
    }

    const string ip = "192.168.100.52";  // IP von deinem Turtlebot / TCP-Server
    CommandClient cmdClient(ip, 9999);   // Port 9999 für Commands

    // 2. Beispiel: erst geradeaus fahren, dann drehen, dann stoppen
    //    v = linear, w = angular

    float v;
    float w;
    float* vPtr = &v;
    float* wPtr = &w;

    // 2.1 Geradeaus fahren (z.B. 3 Sekunden)
    v = 0.1f;   // 0.1 m/s
    w = 0.0f;   // keine Drehung

    for (int i = 0; i < 30; ++i) {  // 30 Befehle, ca. alle 100 ms
        string cmd = buildCmdString(vPtr, wPtr);
        cmdClient.sendCommand(cmd);
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    // 2.2 Drehen auf der Stelle (z.B. 3 Sekunden)
    v = 0.0f;
    w = 0.5f;   // 0.5 rad/s drehen

    for (int i = 0; i < 30; ++i) {
        string cmd = buildCmdString(vPtr, wPtr);
        cmdClient.sendCommand(cmd);
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    // 2.3 Anhalten
    v = 0.0f;
    w = 0.0f;

    for (int i = 0; i < 10; ++i) {
        string cmd = buildCmdString(vPtr, wPtr);
        cmdClient.sendCommand(cmd);
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    // 3. Aufräumen
    cmdClient.close();
    cleanupWinsock();

    cout << "Testfahrt beendet.\n";
    return 0;
}
