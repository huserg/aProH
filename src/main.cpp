#include <Arduino.h>

#include "Led/LedBandController.h"
#include "Web/ServerController.h"


void setup() {
  Serial.begin(115200);
  Serial.println(F("LOGS : Initialize System"));

  SetupWifi();
  SetupLedBand();

  Serial.println(F("LOGS : INIT OK"));
}

void loop() {

  String hexa = ReadSelfLedValue();

  SendData(hexa);

  JSONVar response = RequestData();
  
  UpdateLedBand(response);

}
