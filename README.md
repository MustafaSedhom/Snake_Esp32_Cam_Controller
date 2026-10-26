# Snake ESP32-CAM Controller

ESP32-CAM firmware that serves a live camera stream and provides HTTP APIs for controlling the LED and configuring the camera's Wi-Fi hotspot.

## Companion app

The companion app for this project is available here:

[Snake ESP32-CAM App](https://github.com/MustafaSedhom/Snake_ESP32_Cam_App)

## Features

- Live camera page at `/` and MJPEG stream at `/stream` on port `80`.
- LED control API on port `81` at `/api/led`.
- Hotspot information and configuration API on port `81` at `/api/hotspot`.
- Camera and control servers run in separate FreeRTOS tasks, leaving Arduino's `loop()` empty.

## HTTP API

Replace `<device-ip>` with the ESP32 access point IP address. The firmware reports this address during startup; it is commonly `192.168.4.1` with the default ESP32 SoftAP configuration.

### Camera

- `GET http://<device-ip>/` — camera page.
- `GET http://<device-ip>/stream` — MJPEG camera stream.

### LED

- `GET http://<device-ip>:81/api/led` — read LED state.
- `POST http://<device-ip>:81/api/led` — set brightness or switch the LED on/off.

Example request bodies:

```json
{"brightness":128}
```

```json
{"on":true}
```

Brightness must be from `0` to `255`.

### Hotspot

- `GET http://<device-ip>:81/api/hotspot` — read the hotspot SSID and IP address. The password is not returned.
- `POST http://<device-ip>:81/api/hotspot` — update the SSID, password, or both. The device restarts its hotspot after replying, so the client must reconnect.

Example request body:

```json
{"ssid":"MyCamera","password":"camera1234"}
```

SSID length must be 1–32 characters. Password length must be 8–63 characters.

## Build and upload

This project uses PlatformIO. Select the `esp32cam` environment, then build and upload the firmware:

```sh
pio run -e esp32cam
pio run -e esp32cam -t upload
```

Configure the camera, LED, Wi-Fi hotspot, UART debug, and server settings in the project's configuration headers before building.

## Build memory report

Latest reported PlatformIO usage:

| Memory | Used | Available | Usage |
| --- | ---: | ---: | ---: |
| RAM | 50,012 bytes | 327,680 bytes | 15.3% |
| Flash | 864,401 bytes | 3,145,728 bytes | 27.5% |
