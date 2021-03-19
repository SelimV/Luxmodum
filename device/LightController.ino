#include "LightController.h"

LightController::LightController()
{
    pinMode(LED_BUILTIN, OUTPUT); //set the pin mode for the built-in LED
    pinMode(LED_DATA, OUTPUT);
}

/* void LightController::fillFromPalette()
{
    uint8_t colorIndex=1;
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = ColorFromPalette( state.currentPalette, colorIndex, brightness, state.currentBlending);
        colorIndex += 3;
    }
}  */

//put the given colour to all of the leds (FastLED.show() must be called separately)
void LightController::fillStatic_(CRGB colour)
{
    for (int i = 0; i < NUM_LEDS; ++i)
    {
        leds_[i] = colour;
    }
}

void LightController::start()
{
    //add LEDs to the FastLED
    FastLED.addLeds<LED_TYPE, LED_DATA, COLOR_ORDER>(leds_, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(brightness_);

    ledRest();

    /* state.currentPalette=RainbowColors_p;
    state.currentBlending=LINEARBLEND;
    fillFromPalette();
    FastLED.show(); */
}

void LightController::changeBrightness(int b)
{
    brightness_ += b;
    FastLED.setBrightness(brightness_);
    FastLED.show();
}

void LightController::updateColour(CRGB colour)
{
    fillStatic_(colour);
    FastLED.show();
}

//switch between the modes
void LightController::ledOff()
{
    mode_ = off;
    fillStatic_(colours.off);
    FastLED.show();
}
void LightController::ledRest()
{
    mode_ = rest;
    fillStatic_(colours.rest);
    FastLED.show();
}
void LightController::ledWork()
{
    mode_ = work;
    fillStatic_(colours.work);
    FastLED.show();
}

//pomodoro functions -----------------------------------------
void LightController::setPomodoroTimes(int workTime, int restTime)
{
    workTime_ = workTime;
    restTime_ = restTime;
}
void LightController::startPomodoro(int rounds)
{
    roundsLeft_ = rounds;
    ledWork();
    nextSwitch_ = millis() + workTime_ * TIME_UNIT;
}
void LightController::stopPomodoro()
{
    ledRest();
    roundsLeft_ = 0;
}
bool LightController::checkPomodoro()
{
    return roundsLeft_ > 0;
}
void LightController::updatePomodoro()
{
    if (checkPomodoro())
    {
        if (millis() > nextSwitch_)
        {
            if (mode_ == work)
            {
                roundsLeft_--;
                ledRest();
                nextSwitch_ = millis() + restTime_ * TIME_UNIT;
            }
            else
            {
                ledWork();
                nextSwitch_ = millis() + workTime_ * TIME_UNIT;
            }
        }
    }
}

void LightController::onboardOn()
{
    onboardOn_ = true;               //update the state
    digitalWrite(LED_BUILTIN, HIGH); //turn the onboard LED on
}

void LightController::onboardOff()
{
    onboardOn_ = false;             //update the state
    digitalWrite(LED_BUILTIN, LOW); //turn the onboard LED off
}

void LightController::onboardToggle()
{
    //check the state and call the function that changes it
    if (onboardOn_)
    {
        onboardOff();
    }
    else
    {
        onboardOn();
    }
}
