#include <Arduino.h>
#include "variables.h"
#include "LightController.h"
#include "WiFiController.h"


LightController lightController;    //a controller for the LEDs 
WiFiController wifiController;      //a controller for the WiFi server

//setup runs when the device is started
void setup()
{
    delay( 100 );
    Serial.begin(115200); //open serial
	Serial.println("Starting");  
    lightController.start();
    wifiController.setLightController(&lightController);    //link the light controller to the server
    wifiController.setUpServer();   //connect to WiFi
    delay(10);
}

//this is the main loop of the program
void loop()
{      }
