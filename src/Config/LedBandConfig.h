#pragma once

#define SELF_LED_BLUE 5 //D1
#define SELF_LED_RED 4 //D2
#define SELF_LED_GREEN 0 //D3

const uint16_t PixelCount = 5; 
const uint8_t PixelPin = 3; //RX pin; // For Esp8266, the Pin is omitted and it uses GPIO3 due to DMA hardware use.  
#define colorSaturation 255

#define SELF_POTENTIOMETER A0 //10K potentiometer
const int POTENTIOMETER_MAX_VALUE = 1024;

