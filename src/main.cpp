#include <Arduino.h>
#include <ArduinoJson.h>
#include "Led/LedBandController.h"
#include "Web/ServerController.h"
#include "Config/NetworkConfig.h"
#include "Config/LedBandConfig.h"

// Instantiate the controllers
ServerController serverController(SSID, PASSWORD, SERVER_NAME, HTTP_PORT);
LedBandController ledBandController(PixelCount, PixelPin);

// Variables to keep track of time and values
unsigned long previousMillis = -900000; // Initialize to -900000 to trigger an immediate request on startup
unsigned long serverRequestInterval = 900000; // 15 minutes in milliseconds
String previousHexa = "";

void setup() {
    Serial.begin(115200);
    Serial.println(F("LOGS : Initialize System"));

    // Setup WiFi and LED band
    serverController.setupWifi();
    ledBandController.setup();

    Serial.println(F("LOGS : INIT OK"));
}

void loop() {
  // Read the current LED value from the potentiometer
  String hexa = ledBandController.readSelfLedValue();

  // Send the data to the server only if it has changed
    if (hexa != previousHexa) {
        serverController.sendData(hexa);
        previousHexa = hexa;
    }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= serverRequestInterval) {
      previousMillis = currentMillis;

      // Request data from the server and update the LED band
      JsonDocument response;
      if (serverController.requestData(response)) {
          JsonArray relatedDevices = response["related_devices"].as<JsonArray>();
          if (!relatedDevices.isNull()) {
              ledBandController.updateLedBand(relatedDevices);
          } else {
              Serial.println("Error: related_devices array is null");
          }
      } else {
          Serial.println("Error fetching data from server");
      }
  }

  // Add a small delay to avoid excessive CPU usage
  delay(500); // 100 milliseconds delay between each loop iteration 
}