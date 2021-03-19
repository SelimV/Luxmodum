#include "LightController.h"

LightController::LightController()
{
    pinMode(LED_BUILTIN, OUTPUT); //set the pin mode for the built-in LED
    pinMode(LED_DATA, OUTPUT);
    pinMode(BUTTON, INPUT); //set pin mode for button
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
void LightController::fillStatic_(CHSV colour)
{
    colours.current=colour;
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
    delay(10);
    FastLED.setBrightness(brightness_);
    delay(10);
    FastLED.show();
    delay(10);
}

void LightController::updateColour(CHSV colour)
{
    delay(10);
    fillStatic_(colour);
    delay(10);
    FastLED.show();
    delay(10);
}

//switch between the modes
void LightController::ledOff()
{
    mode_ = off;
    delay(10);
    fillStatic_(colours.off);
    delay(10);
    FastLED.show();
    delay(10);
}
void LightController::ledRest()
{
    mode_ = rest;
    delay(10);
    fillStatic_(colours.rest);
    delay(10);
    FastLED.show();
    delay(10);
}
void LightController::ledWork()
{
    mode_ = work;
    delay(10);
    fillStatic_(colours.work);
    delay(10);
    FastLED.show();
    delay(10);
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

//returns a JSON formatted string of details, including current mode, 
String LightController::details(){
    String json("{");
    json += "\"mode\":";
    json += mode_;

    json += ",\"pomodoro\":";
    json += "{";
            json += "\"workTime\":";
            json += workTime_;
            json += ",\"restTime\":";
            json += restTime_;
            json += ",\"roundsLeft\":";
            json += roundsLeft_;
            json += ",\"active\":";
            json += checkPomodoro();
            json += ",\"timeUntilSwitch\":";
            json += max((int)(nextSwitch_-millis())/TIME_UNIT,0);
    json += "}";

    json += ",\"colours\":";
     json += "{";
            json += "\"work\":";
                json += "{";
                        json += "\"hue\":";
                        json += colours.work.h;
                        json += ",\"saturation\":";
                        json += colours.work.s;
                        json += ",\"brightness\":";
                        json += colours.work.v;
                json += "}";
            
            json += ",\"rest\":";
                json += "{";
                        json += "\"hue\":";
                        json += colours.rest.h;
                        json += ",\"saturation\":";
                        json += colours.rest.s;
                        json += ",\"brightness\":";
                        json += colours.rest.v;
                json += "}";
            json += ",\"current\":";
                json += "{";
                        json += "\"hue\":";
                        json += colours.current.h;
                        json += ",\"saturation\":";
                        json += colours.current.s;
                        json += ",\"brightness\":";
                        json += colours.current.v;
                json += "}";
            
    json += "}";

    json += "}";
    return json;
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

//button functions----------------------------------
void LightController::readButton()
{
    auto buttonState = digitalRead(BUTTON);
    if (buttonState == HIGH)
    {
        stopPomodoro();
        toggleState();
        delay(200);
    }
}

void LightController::toggleState()
{
    if (mode_ == off)
    {
        ledRest();
    }
    else if (mode_ == rest)
    {
        ledWork();
    }
    else
    {
        ledOff();
    }
}
