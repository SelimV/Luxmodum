#pragma once
#include <FastLED.h>

//define the pin numbers
#define LED_DATA 32
#define LED_BUILTIN 2

struct Colours{
    CRGB off=CHSV(0x00,0x00,0x00);
    CRGB work=CHSV(0x00,0x00,0xFF);
    CRGB rest=CHSV(0xC0,0xFF,0xFF);
};

//FastLED defines
#define NUM_LEDS    9
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB
#define UPDATES_PER_SECOND 100