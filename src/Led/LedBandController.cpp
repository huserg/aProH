#include "LedBandController.h"

LedBandController::LedBandController(uint16_t pixelCount, uint8_t pixelPin)
    : strip(pixelCount, pixelPin),
      red(colorSaturation, 0, 0),
      green(0, colorSaturation, 0),
      blue(0, 0, colorSaturation),
      yellow(colorSaturation, colorSaturation, 0),
      purple(colorSaturation, 0, colorSaturation),
      cyan(0, colorSaturation, colorSaturation),
      white(colorSaturation),
      black(0) {}

String LedBandController::toHexString(uint8_t red, uint8_t green, uint8_t blue) {
    char hexString[7];  // 6 characters for colors + 1 for null terminator
    sprintf(hexString, "%02X%02X%02X", red, green, blue);
    return String(hexString);
}

void LedBandController::setup() {
    Serial.println(F("LOGS : Initialize RGB LED"));
    
    // Set all SELF_LED TO OUTPUT
    pinMode(SELF_LED_BLUE, OUTPUT);
    pinMode(SELF_LED_RED, OUTPUT);
    pinMode(SELF_LED_GREEN, OUTPUT);

    // Define potentiometer input
    pinMode(SELF_POTENTIOMETER, INPUT);

    // This resets all the Pixies to an off state
    strip.Begin();
    strip.SetPixelColor(1, cyan);
    strip.Show();
    
    delay(1000);
    
    strip.SetPixelColor(1, black);
    strip.Show();
}

String LedBandController::readSelfLedValue() {
    // If potentiometer is from 0 to 33 -> red, then blue to 66 and green to 100
    int potentiometerValue = analogRead(SELF_POTENTIOMETER);
    int redValue = 0;
    int blueValue = 0;
    int greenValue = 0;

    Serial.print(F("LOGS : POTENTIOMETER VALUE : "));
    Serial.println(potentiometerValue);

    // Get potentiometer value (4095 max)
    if (potentiometerValue <= POTENTIOMETER_MAX_VALUE / 3 * 1) {
        redValue = 255;
    } else if (potentiometerValue <= POTENTIOMETER_MAX_VALUE / 3 * 2) {
        blueValue = 255;
    } else {
        greenValue = 255;
    }

    analogWrite(SELF_LED_RED, redValue);
    analogWrite(SELF_LED_BLUE, blueValue);
    analogWrite(SELF_LED_GREEN, greenValue);

    // Return hexa value of the color when all values are set
    String hexaColor = toHexString(redValue, greenValue, blueValue);

    Serial.print(F("LOGS : Hexa value of COLOR : "));
    Serial.println(hexaColor);

    return hexaColor;
}

void LedBandController::updateLedBand(const JsonArray& relatedDevices) {
    int len = relatedDevices.size();
    for (int i = 0; i < len; i++) {
        JsonObject json_device = relatedDevices[i];
        Device device = Device::fromJSON(json_device);

        // Convert hex color code to RGB values
        long color = strtol(device.getStatus().c_str(), NULL, 16);
        RgbColor ledColor((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);

        // Update the corresponding LED according to the order
        strip.SetPixelColor(i, ledColor);
    }

    // Show the changes
    strip.Show();
}
