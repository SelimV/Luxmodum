#pragma once
#include "variables.h"
#include <FastLED.h>
#include <Arduino.h>
#include <string.h>
enum Mode
{
    off = 0,
    work = 1,
    rest = 2
};




//a class that implements a set of functions for controlling the lights
class LightController
{
private:
    //controller state
    CRGB leds_[NUM_LEDS];
    bool onboardOn_ = false;
    char brightness_ = 64;
    Mode mode_ = work;
    //pomodoro state
    int workTime_ = POMODORO_WORK;
    int restTime_ = POMODORO_REST;
    int roundsLeft_ = 0;
    long nextSwitch_;
    
    //void fillFromPalette();
    void fillStatic_(CHSV); //put the given colour to all of the leds (FastLED.show() must be called separately)

public:
    Colours colours;
    LightController();            //constructor
    ~LightController() = default; //destructor

    //setup the LEDs, to be called during the setup
    void start();

    //increase the brightness by a given amount
    void changeBrightness(int);

    //show a given colour
    void updateColour(CHSV);

    //switch between the modes
    void ledOff();
    void ledRest();
    void ledWork();

    Mode getMode() { return mode_; }; //getter

    //pomodoro functions -----------------------------------------
    void setPomodoroTimes(int workTime = POMODORO_WORK, int restTime = POMODORO_REST);
    //start pomodoro for a given number of rounds
    void startPomodoro(int rounds = POMODORO_ROUNDS);
    
    void stopPomodoro();
    //check if the pomodoro is still active
    bool checkPomodoro();
    //change modes and update counters depending on the time passed
    void updatePomodoro();
    
    //returns a JSON formatted string of details, including current mode, 
    String details();
    //onboard LED functions for debugging -------------------------
    void onboardOn();
    void onboardOff();
    void onboardToggle();
};
