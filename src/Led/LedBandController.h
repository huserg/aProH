#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>
#include <ArduinoJson.h>
#include "../Device/Device.h"
#include "../Config/LedBandConfig.h"


class LedBandController {
public:
    LedBandController(uint16_t pixelCount, uint8_t pixelPin);
    
    void setup();
    String readSelfLedValue();
    void updateLedBand(const JsonArray& relatedDevices);

private:
    String toHexString(uint8_t red, uint8_t green, uint8_t blue);

    NeoPixelBus<NeoGrbFeature, NeoEsp8266DmaWs2812xMethod> strip;

    RgbColor red;
    RgbColor green;
    RgbColor blue;
    RgbColor yellow;
    RgbColor purple;
    RgbColor cyan;
    RgbColor white;
    RgbColor black;
};
