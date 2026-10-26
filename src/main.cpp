// ...................................................................................
#include "Board_Config.h"
#include "Cam_Config.h"
#include "LED_config.h"
#include "WIFI_config.h"
#include "UART_Debug_Config.h"
#include "Servers_config.h"
// ...................................................................................
void setup() 
{
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  Setup_Board();
  Setup_UART_Debug();
  Setup_Camera();
  Setup_LED();
  Setup_HotSpot();
  Setup_Servers();
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  // make live stream work in core 1
  xTaskCreatePinnedToCore( cameraServerTask, "CameraServer", 8192, nullptr, 1, nullptr, 1);
  // make all controls and motor and led and hotspot setting in core 0
  xTaskCreatePinnedToCore( controlServerTask, "ControlServer", 4096, nullptr, 1, nullptr, 0);
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  Debug_msg();
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}
// ...................................................................................
void loop() 
{
  // Empty loop
}
// ...................................................................................