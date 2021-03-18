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
    CRGB leds[NUM_LEDS];
    //void fillFromPalette();
    void fillStatic(CRGB);//put the given colour to all of the leds (FastLED.show() must be called separately)
    void setMode(Mode);

public:
    Colours colours;
    LightController();            //constructor
    ~LightController() = default; //destructor

    //setup the LEDs, to be called during the setup
    void start();

    //increase the brightness by a given amount
    void changeBrightness(int);

    //show a given colour
    void updateColour(CRGB);
    
    //switch between the modes
    void ledOff();
    void ledRest();
    void ledWork();


    Mode getMode(){return state.mode;};//getter

    //use the onboard LED
    void onboardOn();
    void onboardOff();
    void onboardToggle();
};
