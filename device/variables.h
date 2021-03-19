#pragma once
#include <FastLED.h>

//define the pin numbers
#define LED_DATA 32
#define LED_BUILTIN 2
#define BUTTON 4

//default pomodoro parameters
#define POMODORO_WORK 25
#define POMODORO_REST 5
#define POMODORO_ROUNDS 4
//times in seconds: 1000 millisecond in a second
#define TIME_UNIT 1000 

//default colours for the modes
struct Colours{
    CHSV off=CHSV(0x00,0x00,0x00);
    CHSV work=CHSV(0x00,0x00,0xFF);
    CHSV rest=CHSV(0xC0,0xFF,0xFF);
    CHSV current=CHSV(0x00,0x00,0xFF);
};

//FastLED defines
#define NUM_LEDS    9
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB
#define UPDATES_PER_SECOND 100