// .........................................................................
#pragma once
// .........................................................................
#include "Imports.h"
#include "APIs_config.h"
// .........................................................................
// .........................................................................
void Setup_Servers()
{
    // Camera GET and POST APIs
    Camera_Server.on(Home_EndPoint, HTTP_GET, handleRoot);
    Camera_Server.on(Live_Camera_EndPoint, HTTP_GET, handleStream);
    //? Start Camera Server
    Camera_Server.begin();

    // LED GET and POST APIs
    Control_Server.on(Led_Control_EndPoint, HTTP_GET, handleLedApi);
    Control_Server.on(Led_Control_EndPoint, HTTP_POST, handleLedApi);
    
    // HotSpot GET and POST APIs
    Control_Server.on(Hotspot_EndPoint, HTTP_GET, handleHotspotApi);
    Control_Server.on(Hotspot_EndPoint, HTTP_POST, handleHotspotApi);
    
    // Restart Board GET and POST APIs
    Control_Server.on(Restart_Board_EndPoint, HTTP_POST,handleRestartApi);
    //? Start Control Server
    Control_Server.begin();
}
// .........................................................................
void cameraServerTask(void *parameter)
{
  for (;;)
  {
    Camera_Server.handleClient();  // Camera stream Server handler
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}
// .........................................................................
void controlServerTask(void *parameter)
{
  for (;;)
  {
    Control_Server.handleClient();  // control Server handler
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}
// .........................................................................
// .........................................................................