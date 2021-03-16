#pragma once
#include "variables.h"
#include <FastLED.h>

enum Mode
{
    off,
    work,
    rest
};

//the current state of the LEDs
struct State
{
    bool onboardOn = false;
    char brightness = 64;
    Mode mode = work;
    CRGBPalette16 currentPalette;
    TBlendType currentBlending;
};

//a class that implements a set of functions for controlling the lights
class LightController
{
private:
    State state;
    Colours colours;
    CRGB leds[NUM_LEDS];
    void fillFromPalette();
    void fillStatic(CRGB);

public:
    LightController();            //constructor
    ~LightController() = default; //destructor

    //setup the LEDs, to be called during the setup
    void start();
    void changeBrightness(char);
    
    void ledOff();
    void ledRest();
    void ledWork();

    
    //use the onboard LED
    void onboardOn();
    void onboardOff();
    void onboardToggle();
};
