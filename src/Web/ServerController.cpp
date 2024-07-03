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

void processResponse(const String& payload, const String& requestType) {
    JSONVar json_payload = JSON.parse(payload);
    if (JSON.typeof(json_payload) == "undefined") {
        Serial.print("NETWORK : WARN: Parsing input failed! - Payload : ");
        Serial.println(payload);
        return;
    }

    if (requestType == "status") {
        // Traitement des réponses de type "status"
        // ? 

    } else if (requestType == "update") {
        // Traitement des réponses de type "update"
        String update_status = JSON.stringify(json_payload["update"]);

        if (update_status == "\"success\"") {
            Serial.println("NETWORK : INFO: Update successful");
        } else if (update_status == "\"error\"") {
            Serial.println("NETWORK : ERROR: Update failed");
        }

    } else {
        // Traitement des réponses génériques
        int code = JSON.stringify(json_payload["code"]).toInt();
        String message = JSON.stringify(json_payload["message"]);
        String new_token = (const char*) json_payload["register_token"];

        switch(code) {
            case 0:
                Serial.println("NETWORK : INFO: Operation successful");
                break;
            case 9:
                Serial.println("NETWORK : ERROR: Invalid payload");
                break;
            case 10:
                Serial.println("NETWORK : ERROR: Missing MAC in payload");
                break;
            case 11:  
                Serial.println("NETWORK : ERROR: MAC not found in DB : " + MAC_ADDRESS);
                break;
            case 12:  
                Serial.println("NETWORK : ERROR: Wrong Token");
                break;
            case 50:
                token = new_token;
                Serial.println("NETWORK : INFO: Token received");
                break;
            default :
                Serial.print("NETWORK : ERROR: Unknown code - ");
                Serial.println(code);
                break;
        }
    }
}

String RequestData() {
    Serial.println("REMINDER : MAC Address : " + MAC_ADDRESS);
    
    String body = "{\"mac\":\"" + MAC_ADDRESS + "\",\"token\":\"" + token + "\"}";
    
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
    
    processResponse(payload, "status");

    // Return the payload
    return payload;
}

void SendData(String new_value) {
    String body = "{\"mac\":\"" + MAC_ADDRESS + "\",\"value\":\"" + new_value + "\",\"token\":\"" + token + "\"}";

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
    
    processResponse(payload, "update");
}
