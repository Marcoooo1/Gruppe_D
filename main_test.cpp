#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

#include "TCP_Connect.h"        // CommandClient 9999  :contentReference[oaicite:0]{index=0}
#include "CMD_Parser.h"         // buildCmdString       :contentReference[oaicite:1]{index=1}
#include "parser_odom.h"        // parseOdomFromJson    :contentReference[oaicite:2]{index=2}
#include "controller.h"         // computeVelocity       :contentReference[oaicite:3]{index=3}

using namespace std;

// ---- Ziele für ein 1×1m Quadrat ----
// Start: (0,0)
// Dann: (1,0) → (1,1) → (0,1) → (0,0)
Pose goals[4] = {
    {1.0f, 0.0f, 0.0f},
    {1.0f, 1.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 0.0f}
};

// Ziel erreicht?
bool reachedGoal(const Pose &cur, const Pose &goal)
{
    float dx = cur.x - goal.x;
    float dy = cur.y - goal.y;
    float dist = sqrt(dx*dx + dy*dy);
    return dist < 0.07f;   // 7 cm Toleranz
}

int main()
{
    // Winsock für Windows
    if (!initWinsock()) return -1;

    string robotIP = "192.168.100.5X";  // <-- deine IP eintragen!
    CommandClient cmd(robotIP, 9999);

    cout << "Starte Quadratfahr-Programm..." << endl;

    // ---- Odom Reader starten (einmaliger Aufruf reicht, wenn JSON bereits kommt) ----
    // Bei Bedarf kannst du wie im TCP_Connect-File einen OdomReader-Thread starten.
    // Für das einfache Beispiel hier holen wir die Odom-Daten aus odom_output.json.

    float odom[3] = {0,0,0};

    for (int i = 0; i < 4; ++i)
    {
        cout << "\n Fahre zu Ziel " << i+1 << endl;

        while (true)
        {
            // ---------------------------------------
            // ODOM EINLESEN
            // ---------------------------------------
            if (!parseOdomFromJson("odom_output.json", odom))
            {
                cout << "Warte auf Odometry..." << endl;
                this_thread::sleep_for(200ms);
                continue;
            }

            Pose current{odom[0], odom[1], odom[2]};

            // Ziel erreicht?
            if (reachedGoal(current, goals[i]))
            {
                cout << "🎯 Ziel erreicht!" << endl;
                break;
            }

            // ---------------------------------------
            // VELOCITY BERECHNEN
            // ---------------------------------------
            auto vel = computeVelocity(current, goals[i]);

            // JSON-Command-String bauen
            string cmdStr = buildCmdString(&vel->v, &vel->w);

            // ---------------------------------------
            // AN TURTLEBOT SCHICKEN
            // ---------------------------------------
            cmd.sendCommand(cmdStr);

            this_thread::sleep_for(100ms);
        }
    }

    // STOP senden
    float vStop = 0.0f, wStop = 0.0f;
    cmd.sendCommand(buildCmdString(&vStop, &wStop));

    cout << "\nQuadrat abgefahren!" << endl;

    cleanupWinsock();
    return 0;
}
