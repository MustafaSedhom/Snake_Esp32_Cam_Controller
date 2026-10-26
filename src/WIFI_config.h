// ........................................................................
#pragma once
// ........................................................................
#include "Imports.h"
#include "APIs_config.h"
// ........................................................................
// ........................................................................
void Setup_HotSpot()
{
    WiFi.mode(WIFI_AP);
    // init static IP
    if (!WiFi.softAPConfig(localIP, gateway, subnet)) {
        Serial.println("Failed to configure hotspot IP");
        return;
    }
    // Start the hotspot with the current SSID and password.
    bool apStarted = WiFi.softAP(apSsid.c_str(), apPassword.c_str());

    if (!apStarted) {
    Serial.println("Could not start Wi-Fi hotspot");
    while (true) {
        delay(1000);
    }
    }
}
// ........................................................................