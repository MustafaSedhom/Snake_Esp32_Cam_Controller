// .........................................................................
#pragma once
// .........................................................................
#include "Imports.h"
// .........................................................................
WebServer server(80);        // Camera page and stream
WebServer controlServer(81); // LED and hotspot APIs
// .........................................................................
void handleRoot(void);
void handleStream(void);

void handleLedApi(void);

void handleHotspotApi(void);
// .........................................................................
void Setup_Servers()
{
    server.on("/", HTTP_GET, handleRoot);
    server.on("/stream", HTTP_GET, handleStream);
    server.begin();

    controlServer.on("/api/led", HTTP_GET, handleLedApi);
    controlServer.on("/api/led", HTTP_POST, handleLedApi);
    controlServer.on("/api/led", HTTP_OPTIONS, handleLedApi);

    controlServer.on("/api/hotspot", HTTP_GET, handleHotspotApi);
    controlServer.on("/api/hotspot", HTTP_POST, handleHotspotApi);
    controlServer.on("/api/hotspot", HTTP_OPTIONS, handleHotspotApi);
    controlServer.begin();
}
// .........................................................................
void Handling_Severs()
{
    server.handleClient();
    delay(2);
}
// .........................................................................
// .........................................................................