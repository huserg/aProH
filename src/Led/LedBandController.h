#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>
#include <JSONVar.h>
#include <JSON.h>
#include "../Device/Device.h"
#include "../Config/LedBandConfig.h"

extern RgbColor red;
extern RgbColor green;
extern RgbColor blue;
extern RgbColor white;
extern RgbColor black;
extern RgbColor yellow;
extern RgbColor purple;
extern RgbColor cyan;

void SetupLedBand();
String ReadSelfLedValue();
void UpdateLedBand(String data);

