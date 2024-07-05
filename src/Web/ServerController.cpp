#include "ServerController.h"

const char* GET_DATA_PATH = "/api/devices/data";
const char* UPDATE_DATA_PATH = "/api/devices/data/update";


ServerController::ServerController(const char* ssid, const char* password, const char* serverName, uint16_t httpPort)
    : SSID(ssid), PASSWORD(password), SERVER_NAME(serverName), HTTP_PORT(httpPort), isWifiActive(false) {}

void ServerController::setupWifi() {
    Serial.println(F("LOGS : Initialize Wifi"));

    WiFi.begin(SSID, PASSWORD);
    int tries = 50;
    while (WiFi.status() != WL_CONNECTED && tries > 0) {
        delay(500);
        tries--;
    }

    isWifiActive = WiFi.isConnected();
    if (!isWifiActive) {
        Serial.println("NETWORK : ERROR: Connection failed");
        return;
    }

    Serial.println(F("LOGS : Wifi initialized"));

    // Ignore SSL certificate validation
    client.setInsecure();

    MAC_ADDRESS = WiFi.macAddress();

    Serial.print(F("LOGS : MAC Address : "));
    Serial.println(WiFi.localIP().toString());
}

bool ServerController::processResponse(JsonDocument& doc) {
    int code = doc["code"];
    String message = doc["message"].as<String>();

    switch (code) {
        case 0:
            Serial.println("NETWORK : INFO: Operation successful");
            return true;
        case 9:
            Serial.println("NETWORK : ERROR: Invalid payload");
            return false;
        case 10:
            Serial.println("NETWORK : ERROR: Missing MAC in payload");
            return false;
        case 11:
            Serial.println("NETWORK : ERROR: MAC not found in DB : " + MAC_ADDRESS);
            return false;
        default:
            Serial.print("NETWORK : ERROR: Unknown code - ");
            Serial.println(code);
            return false;
    }
}

bool ServerController::requestData(JsonDocument& doc) {
    Serial.println("REMINDER : MAC Address : " + MAC_ADDRESS);

    String body = "{\"mac\":\"" + MAC_ADDRESS + "\"}";

    http.begin(client, SERVER_NAME, HTTP_PORT, "/api/devices/data", true);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(body);

    if (httpResponseCode > 0) {
        String payload = http.getString();
        http.end();

        Serial.print("NETWORK : INFO: Data fetched successfully : ");
        Serial.println(payload);

        DeserializationError error = deserializeJson(doc, payload);
        if (error) {
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.c_str());
            return false;
        }

        return processResponse(doc);
    } else {
        Serial.print("NETWORK : ERROR: Server unreachable - Code : ");
        Serial.println(httpResponseCode);
        http.end();
        return false;
    }
}

void ServerController::sendData(const String& newValue) {
    String body = "{\"mac\":\"" + MAC_ADDRESS + "\",\"status\":\"" + newValue + "\"}";

    http.begin(client, SERVER_NAME, HTTP_PORT, "/api/devices/data/update", true);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(body);

    if (httpResponseCode > 0) {
        String payload = http.getString();
        http.end();

        StaticJsonDocument<256> doc;
        DeserializationError error = deserializeJson(doc, payload);
        if (error) {
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.c_str());
            return;
        }

        bool success = processResponse(doc);
        if (success)
            Serial.println("NETWORK : INFO: Data sent successfully");
        else
            Serial.println("NETWORK : ERROR: Data sending failed");
    } else {
        Serial.print("NETWORK : ERROR: Server unreachable - Code : ");
        Serial.println(httpResponseCode);
        http.end();
    }
}