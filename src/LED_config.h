// ......................................................................................
#pragma once
// ......................................................................................
#include "Imports.h"
#include "Board_Config.h"
// ......................................................................................
void Set_Led_Brightness(int);
void Setup_LED(void);
// ......................................................................................
int ledBrightness = 0;
// ......................................................................................
void Setup_LED()
{
    // PWM API differs between Arduino-ESP32 2.x and 3.x.
    #if ESP_ARDUINO_VERSION_MAJOR >= 3
        ledcAttach(LED_PIN, PWM_FREQ, PWM_RESOLUTION);
    #else
        ledcSetup(LED_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
        ledcAttachPin(LED_PIN, LED_CHANNEL);
    #endif
        Set_Led_Brightness(0);
}
// ......................................................................................
void Set_Led_Brightness(int value) 
{
    ledBrightness = constrain(value, 0, 255);

    #if ESP_ARDUINO_VERSION_MAJOR >= 3
        ledcWrite(LED_PIN, ledBrightness);
    #else
        ledcWrite(LED_CHANNEL, ledBrightness);
    #endif
}
// ......................................................................................