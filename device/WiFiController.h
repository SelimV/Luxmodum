#pragma once

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <string.h>
#include <FastLED.h>
#include "wificredentials.h" // WiFi credentials of your network
#include "variables.h"
#include "LightController.h"

class WiFiController
{
private:
    AsyncWebServer server_;
    LightController *lightController_;
public:
    WiFiController(): server_(80){};  //constructor: initialise the server at port 80
    ~WiFiController() = default; //destructor
    void setLightController(LightController *x) { lightController_ = x; };//setter, call during the setup to attach a light controller
    //connect to wifi and start listening for connections 
    void setUpServer();   
};
