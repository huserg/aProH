#pragma once

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h> 
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

class ServerController {
public:
    ServerController(const char* ssid, const char* password, const char* serverName, uint16_t httpPort);

    void setupWifi();
    bool requestData(JsonDocument& doc);
    void sendData(const String& newValue);

private:
    bool processResponse(JsonDocument& doc);

    const char* SSID;
    const char* PASSWORD;
    const char* SERVER_NAME;
    uint16_t HTTP_PORT;

    String MAC_ADDRESS;
    HTTPClient http;
    WiFiClientSecure client;
    bool isWifiActive;
};

extern const char* GET_DATA_PATH;
extern const char* UPDATE_DATA_PATH;
