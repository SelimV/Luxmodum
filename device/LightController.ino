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

void LightController::fillStatic(CRGB colour)
{
    for (int i = 0; i < NUM_LEDS; ++i)
    {
        leds[i] = colour;
    }
}

void LightController::start()
{
    //add LEDs to the FastLED
    FastLED.addLeds<LED_TYPE, LED_DATA, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(state.brightness);

    ledRest();

    /* state.currentPalette=RainbowColors_p;
    state.currentBlending=LINEARBLEND;
    fillFromPalette();
    FastLED.show(); */
}

void LightController::changeBrightness(char b)
{
    state.brightness += b;
    FastLED.setBrightness(state.brightness);
    FastLED.show();
}

void LightController::ledOff()
{
    fillStatic(colours.off);
    FastLED.show();
}
void LightController::ledRest()
{
    fillStatic(colours.rest);
    FastLED.show();
}
void LightController::ledWork()
{
    fillStatic(colours.work);
    FastLED.show();
}

void LightController::onboardOn()
{
    state.onboardOn = true;          //update the state
    digitalWrite(LED_BUILTIN, HIGH); //turn the onboard LED on
}

void LightController::onboardOff()
{
    state.onboardOn = false;        //update the state
    digitalWrite(LED_BUILTIN, LOW); //turn the onboard LED off
}

void LightController::onboardToggle()
{
    //check the state and call the function that changes it
    if (state.onboardOn)
    {
        onboardOff();
    }
    else
    {
        onboardOn();
    }
}
