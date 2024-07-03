#include "LedBandController.h"


RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor yellow(colorSaturation, colorSaturation, 0);
RgbColor purple(colorSaturation, 0, colorSaturation);
RgbColor cyan(0, colorSaturation, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);


NeoPixelBus<NeoGrbFeature, NeoEsp8266DmaWs2812xMethod> strip(PixelCount, PixelPin);


String toHexString(uint8_t red, uint8_t green, uint8_t blue) {
    char hexString[7];  // 6 caractères pour les couleurs + 1 pour le caractère nul
    sprintf(hexString, "%02X%02X%02X", red, green, blue);
    return String(hexString);
}


void SetupLedBand() {
    Serial.println(F("LOGS : Initialize RGB LED"));
        
    // set all SELF_LED TO OUTPUT
    pinMode(SELF_LED_BLUE, OUTPUT);
    pinMode(SELF_LED_RED, OUTPUT);
    pinMode(SELF_LED_GREEN, OUTPUT);

    // define potentiometer input : 
    pinMode(SELF_POTENTIOMETER, INPUT);
    
    
    // this resets all the Pixies to an off state
    strip.Begin();
    strip.SetPixelColor(1, cyan);
    strip.Show();
    
    delay(1000);
    
    strip.SetPixelColor(1, black);
    strip.Show();
}

String ReadSelfLedValue() {
    // if potentiometer is from 0 to 33 -> red, then blue to 66 and green to 100
    int potentiometerValue = analogRead(SELF_POTENTIOMETER);
    int redValue = 0;
    int blueValue = 0;
    int greenValue = 0;

    Serial.print(F("LOGS : POTENTIOMETER VALUE : "));
    Serial.println(potentiometerValue);

    // get potentiometer value (4095 max)
    if (potentiometerValue <= POTENTIOMETER_MAX_VALUE/3*1) {
        greenValue = 255;
    } else if (potentiometerValue <= POTENTIOMETER_MAX_VALUE/3*2) {
        blueValue = 255;
    } else {
        redValue = 255;
    }

    analogWrite(SELF_LED_RED, redValue);
    analogWrite(SELF_LED_BLUE, blueValue);
    analogWrite(SELF_LED_GREEN, greenValue);

    // return hexa value of the color when all values are set
    String hexaColor = toHexString(redValue, greenValue, blueValue);

    Serial.print(F("LOGS : Hexa value of COLOR : "));
    Serial.println(hexaColor);

    return hexaColor;
}

void UpdateLedBand(String data) {
    JSONVar json_data = JSON.parse(data);

    if (JSON.typeof(json_data) == "undefined") {
        Serial.println("Error parsing JSON");
        return;
    }

    // Supposons que "data" soit un tableau JSON
    int len = json_data.length();
    for (int i = 0; i < len; i++) {
        JSONVar item = json_data[i];
        Device device = Device::fromJSON(item);

        // Convertir le code couleur hexa en valeurs RGB
        long color = strtol(device.getStatus().c_str(), NULL, 16);
        RgbColor ledColor((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);

        // Mettre à jour la LED correspondante selon l'ordre
        strip.SetPixelColor(device.getOrder(), ledColor);
    }

    // Afficher les changements
    strip.Show();

}