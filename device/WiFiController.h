#pragma once

#include <WiFi.h>
#include <ESPmDNS.h>
#include "wificredentials.h" // WiFi credentials of your network
#include "variables.h"
#include "LightController.h"

class WiFiController
{
private:
    WiFiServer server_;
    LightController *lightController_;
    //advertise the service to the network
    void advertise();
    //react to the read request line
    void handleRequest(String line);

public:
    WiFiController() = default;  //constructor
    ~WiFiController() = default; //destructor
    void setLightController(LightController *x) { lightController_ = x; };//setter, call during the setup to attach a light controller
    //connect to wifi and start listening for connections 
    void setUpServer();   
    //react to incoming connections
    void handleClient();
    
};
