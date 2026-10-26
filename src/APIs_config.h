// .........................................................................
#pragma once
// .........................................................................
#include "Imports.h"
// .........................................................................
// .........................................................................
WebServer Camera_Server(80);        // Camera stream Server
WebServer Control_Server(81);       // Control Server
// .........................................................................
#define   Home_EndPoint               "/"
#define   Live_Camera_EndPoint        "/stream"
#define   Led_Control_EndPoint        "/api/led"
#define   Hotspot_EndPoint            "/api/hotspot"
#define   Restart_EndPoint            "/api/restart"
// .........................................................................
void handleLedApi() {
  Control_Server.sendHeader("Access-Control-Allow-Origin", "*");

  if (Control_Server.method() == HTTP_OPTIONS) {
    Control_Server.sendHeader(
      "Access-Control-Allow-Methods",
      "GET, POST, OPTIONS"
    );
    Control_Server.sendHeader(
      "Access-Control-Allow-Headers",
      "Content-Type"
    );
    Control_Server.send(204);
    return;
  }

  // GET /api/led returns the current LED state.
  if (Control_Server.method() == HTTP_GET) {
    String response = "{\"ok\":true,\"brightness\":";
    response += String(ledBrightness);
    response += ",\"on\":";
    response += (ledBrightness > 0) ? "true}" : "false}";
    Control_Server.send(200, "application/json", response);
    return;
  }

  // POST /api/led accepts {"brightness":128} or {"on":true}.
  if (!Control_Server.hasArg("plain")) {
    Control_Server.send(
      400,
      "application/json",
      "{\"ok\":false,\"error\":\"Expected JSON body\"}"
    );
    return;
  }

  String body = Control_Server.arg("plain");
  int key = body.indexOf("\"brightness\"");

  if (key >= 0) {
    int colon = body.indexOf(':', key);

    if (colon < 0) {
      Control_Server.send(
        400,
        "application/json",
        "{\"ok\":false,\"error\":\"Invalid brightness\"}"
      );
      return;
    }

    String valueText = body.substring(colon + 1);
    valueText.trim();

    int value = valueText.toInt();
    if (value < 0 || value > 255) {
      Control_Server.send(
        400,
        "application/json",
        "{\"ok\":false,\"error\":\"brightness must be 0..255\"}"
      );
      return;
    }

    Set_Led_Brightness(value);
  } else {
    key = body.indexOf("\"on\"");
    int colon = (key >= 0) ? body.indexOf(':', key) : -1;

    if (colon < 0) {
      Control_Server.send(
        400,
        "application/json",
        "{\"ok\":false,\"error\":\"Use brightness or on\"}"
      );
      return;
    }

    String value = body.substring(colon + 1);
    value.trim();

    if (value.startsWith("true")) {
      Set_Led_Brightness(255);
    } else if (value.startsWith("false")) {
      Set_Led_Brightness(0);
    } else {
      Control_Server.send(
        400,
        "application/json",
        "{\"ok\":false,\"error\":\"on must be true or false\"}"
      );
      return;
    }
  }

  String response = "{\"ok\":true,\"brightness\":";
  response += String(ledBrightness);
  response += ",\"on\":";
  response += (ledBrightness > 0) ? "true}" : "false}";
  Control_Server.send(200, "application/json", response);
}
// .........................................................................
// Reads simple, unescaped JSON string values such as "ssid":"MyCamera".
bool readJsonString(const String& body, const char* key, String& value) {
  String search = String("\"") + key + "\"";
  int keyPos = body.indexOf(search);
  if (keyPos < 0) {
    return false;
  }

  int colon = body.indexOf(':', keyPos + search.length());
  if (colon < 0) {
    return false;
  }

  int firstQuote = body.indexOf('"', colon + 1);
  if (firstQuote < 0) {
    return false;
  }

  int lastQuote = body.indexOf('"', firstQuote + 1);
  if (lastQuote < 0) {
    return false;
  }

  value = body.substring(firstQuote + 1, lastQuote);
  return true;
}
// .........................................................................
void handleHotspotApi() {
  Control_Server.sendHeader("Access-Control-Allow-Origin", "*");

  if (Control_Server.method() == HTTP_OPTIONS) {
    Control_Server.sendHeader(
      "Access-Control-Allow-Methods",
      "GET, POST, OPTIONS"
    );
    Control_Server.sendHeader(
      "Access-Control-Allow-Headers",
      "Content-Type"
    );
    Control_Server.send(204);
    return;
  }

  // GET /api/hotspot returns the SSID and IP, but not the password.
  if (Control_Server.method() == HTTP_GET) {
    String response = "{\"ok\":true,\"ssid\":\"";
    response += apSsid;
    response += "\",\"ip\":\"";
    response += WiFi.softAPIP().toString();
    response += "\"}";

    Control_Server.send(200, "application/json", response);
    return;
  }

  // POST /api/hotspot accepts {"ssid":"...","password":"..."}.
  if (!Control_Server.hasArg("plain")) {
    Control_Server.send(
      400,
      "application/json",
      "{\"ok\":false,\"error\":\"Expected JSON body\"}"
    );
    return;
  }

  String body = Control_Server.arg("plain");
  String newSsid;
  String newPassword;

  bool hasSsid = readJsonString(body, "ssid", newSsid);
  bool hasPassword = readJsonString(body, "password", newPassword);

  if (!hasSsid && !hasPassword) {
    Control_Server.send(
      400,
      "application/json",
      "{\"ok\":false,\"error\":\"Provide ssid or password\"}"
    );
    return;
  }

  if (hasSsid && (newSsid.length() < 1 || newSsid.length() > 32)) {
    Control_Server.send(
      400,
      "application/json",
      "{\"ok\":false,\"error\":\"ssid must be 1..32 characters\"}"
    );
    return;
  }

  if (hasPassword &&
      (newPassword.length() < 8 || newPassword.length() > 63)) {
    Control_Server.send(
      400,
      "application/json",
      "{\"ok\":false,\"error\":\"password must be 8..63 characters\"}"
    );
    return;
  }

  // Fields you omit keep their current values.
  if (hasSsid) {
    apSsid = newSsid;
  }
  if (hasPassword) {
    apPassword = newPassword;
  }

  // Reply first. The hotspot then restarts and the client disconnects.
  Control_Server.send(
    200,
    "application/json",
    "{\"ok\":true,\"message\":\"Hotspot restarting; reconnect with the new settings\"}"
  );

  delay(250);
  WiFi.softAPdisconnect(true);
  delay(300);

  if (!WiFi.softAP(apSsid.c_str(), apPassword.c_str())) {
    Serial.println("Failed to restart hotspot with new settings");
  } else {
    Serial.printf("Hotspot updated: %s\n", apSsid.c_str());
    Serial.printf(
      "Hotspot IP: %s\n",
      WiFi.softAPIP().toString().c_str()
    );
  }
}
// .........................................................................
void handleRoot() {
  const char PAGE[] PROGMEM = R"HTML(
    <!doctype html>
    <html>
    <head>
      <meta name="viewport" content="width=device-width,initial-scale=1">
      <title>ESP32-CAM Live Stream Robotics & SEDHOM </title>
      <style>
        body {
          font-family: Arial, sans-serif;
          text-align: center;
          background: #111;
          color: #07cee0;
          margin: 1rem;
        }

        img {
          width: 100%;
          max-width: 800px;
          height: auto;
          border: 1px solid #15ff00;
        }
      </style>
    </head>
    <body>
      <h2>ESP32-CAM Live Stream</h2>
      <img src="/stream" alt="Live camera stream">
    </body>
    </html>
    )HTML";
  Camera_Server.send_P(200, "text/html", PAGE);
}
// .........................................................................
void handleStream() {
  WiFiClient client = Camera_Server.client();

  client.print("HTTP/1.1 200 OK\r\n");
  client.print(
    "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n"
  );
  client.print("Cache-Control: no-cache\r\n");
  client.print("Connection: close\r\n\r\n");

  while (client.connected()) {
    camera_fb_t* fb = esp_camera_fb_get();

    if (!fb) {
      Serial.println("Camera capture failed");
      break;
    }

    client.print("--frame\r\n");
    client.print("Content-Type: image/jpeg\r\n");
    client.printf("Content-Length: %u\r\n\r\n", (unsigned int)fb->len);

    size_t sent = client.write(fb->buf, fb->len);
    client.print("\r\n");

    esp_camera_fb_return(fb);

    if (sent == 0) {
      break;
    }

    delay(30);
  }

  client.stop();
}
// .........................................................................
// .........................................................................
// .........................................................................
