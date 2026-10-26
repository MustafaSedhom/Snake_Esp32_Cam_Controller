// ...........................................................
#pragma once
// ...........................................................
#include "Imports.h"
#include "WIFI_config.h"
// ...........................................................
void Setup_UART_Debug()
{
    Serial.begin(115200);
    Serial.println("Start Debuging .....");
}
// ...........................................................
void Debug_msg()
{
    Serial.printf("Hotspot: %s\n", apSsid.c_str());
    Serial.print("Open stream page: http://");
    Serial.println(WiFi.softAPIP());
    Serial.println("Control APIs: http://192.168.4.1:81/");
}
// ...........................................................