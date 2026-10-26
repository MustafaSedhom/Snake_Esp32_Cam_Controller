// ........................................................................
#pragma once
// ........................................................................
#include "Imports.h"
// ........................................................................
// AI-Thinker ESP32-CAM hotspot defaults
String apSsid = "ESP32-CAM";
String apPassword = "1234567890";
// ........................................................................
void Setup_HotSpot()
{
    // Start the hotspot with the current SSID and password.
    WiFi.mode(WIFI_AP);
    bool apStarted = WiFi.softAP(apSsid.c_str(), apPassword.c_str());

    if (!apStarted) {
    Serial.println("Could not start Wi-Fi hotspot");
    while (true) {
        delay(1000);
    }
    }
}
// ........................................................................