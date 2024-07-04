#include "ServerController.h"

// WEB SERVER Data
const String HTTP_METHOD = "POST";
const String GET_DATA_PATH = "/api/devices/data";
const String UPDATE_DATA_PATH = "/api/devices/data/update";

String MAC_ADDRESS = ""; // will be queried from wifi    
String token = "";
HTTPClient http;
bool isWWifiActive = false;
WiFiClientSecure client;


void SetupWifi() {
    Serial.println(F("LOGS : Initialize Wifi"));
        
    WiFi.begin(SSID, PASSWORD);
    int tries = 50;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        tries -= 1;

        if (tries <= 0) {
            Serial.println("NETWORK : ERROR: Connection failed");
            break;
        }
    }

    isWWifiActive = WiFi.isConnected();
    
    Serial.println(F("LOGS : Wifi initialized"));

    // Ignore SSL certificate validation
    client.setInsecure();

    MAC_ADDRESS = WiFi.macAddress();
    
    Serial.print(F("LOGS : MAC Address : "));
    Serial.println(WiFi.localIP().toString());
}

bool processResponse(JSONVar payload, const String requestType) {
    //JSONVar json_payload = JSON.parse(payload);
    //if (JSON.typeof(json_payload) == "undefined") {
    //    Serial.println("NETWORK : WARN: Parsing input failed! - Payload not a JSON");
    //    return false;
    //}

    // Traitement des réponses génériques
    int code = JSON.stringify(payload["code"]).toInt();
    String message = JSON.stringify(payload["message"]);

    switch(code) {
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
        default :
            Serial.print("NETWORK : ERROR: Unknown code - ");
            Serial.println(code);
            return false;
    }
}

JSONVar RequestData() {
    Serial.println("REMINDER : MAC Address : " + MAC_ADDRESS);
    
    String body = "{\"mac\":\"" + MAC_ADDRESS + "\"}";
    
    http.begin(client, SERVER_NAME, HTTP_PORT, GET_DATA_PATH, true);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(body);
    
    String payload = "null";
    if (httpResponseCode > 0) {
        // Get the request response payload
        payload = http.getString();
    } else {
        Serial.print("NETWORK : ERROR: Server unreachable - Code : ");
        Serial.println(httpResponseCode);
        return "null";
    }
    
    // Free resources
    http.end();
    
    Serial.print("NETWORK : INFO: Data fetched successfully : ");
    Serial.println(payload);

    JSONVar json_payload = JSON.parse(payload);
    if (JSON.typeof(json_payload) == "undefined") {
        Serial.println("NETWORK : WARN: Parsing input failed! - Payload not a JSON");
        return false;
    }

    bool success = processResponse(payload, "status");
    if (!success) {
        Serial.println("NETWORK : ERROR: Data fetching failed");
        return {};
    } else {
        Serial.println("NETWORK : INFO: Data fetched successfully");
        return json_payload;
    }
}

void SendData(String new_value) {
    String body = "{\"mac\":\"" + MAC_ADDRESS + "\",\"status\":\"" + new_value + "\"}";

    http.begin(client, SERVER_NAME, HTTP_PORT, UPDATE_DATA_PATH, true);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(body);

    String payload = "null";
    if (httpResponseCode > 0) {
        // Get the request response payload
        payload = http.getString();
    } else {
        Serial.print("NETWORK : ERROR: Server unreachable - Code : ");
        Serial.println(httpResponseCode);
        return;
    }

    // Free resources
    http.end();
    
    JSONVar json_payload = JSON.parse(payload);
    if (JSON.typeof(json_payload) == "undefined") {
        Serial.println("NETWORK : WARN: Parsing input failed! - Payload not a JSON");
        return;
    }

    bool success = processResponse(payload, "update");

    if (success)
        Serial.println("NETWORK : INFO: Data sent successfully");
    else
        Serial.println("NETWORK : ERROR: Data sending failed");
}
