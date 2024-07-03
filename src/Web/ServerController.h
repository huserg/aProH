#pragma once

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h> 
#include <WiFiClientSecure.h>
#include <Arduino_JSON.h>
#include "../Config/NetworkConfig.h"


void SetupWifi();
String RequestData();
void SendData(String new_value);
