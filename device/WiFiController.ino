#include "WiFiController.h"

void WiFiController::setUpServer()
{
    //connect to wifi
    WiFi.begin(ssid, password);
    //wait until the connection is ready
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    //begin the server that listens to incoming connections
    server_.begin();
    //advertise the service to the network
    advertise();
}

void WiFiController::advertise()
{
    //Start the service
    if (MDNS.begin(serviceName))
    {
        MDNS.addService("http", "tcp", 23);
    }
    else
    {
        //something went wrong, stalling
        while (true)
        {
            Serial.println("Error setting up MDNS responder");
            delay(1000);
        }
    }
}

void WiFiController::handleClient()
{
    //check for incoming connections
    WiFiClient client = server_.available();
    if (client)
    {
        //read incoming data
        String line = "";
        while (client.connected())
        {
            //check whether there is data to read
            if (client.available())
            {
                char c = client.read(); //read a byte
                if (c == '\n')          //if it is new line character
                {
                    //if there are two in a row, end of request
                    if (line.length() == 0)
                    {
                        //return OK
                        client.println("HTTP/1.1 200 OK");
                        //we are done
                        break;
                    }
                    else
                    {
                        //otherwise empty the line
                        line = "";
                    }
                }
                else if (c != '\r') //ignore these characters
                {
                    //add the new character
                    line += c;
                }
                //react to the request
                handleRequest(line);
            } //if(client.available())
        }     //while(client.connected())
        //close the connection
        client.stop();
    } //if(client)
}

void WiFiController::handleRequest(String line)
{
    if (line.endsWith("GET /T"))
    {
        lightController_->onboardToggle();
    }
    else if (line.endsWith("GET /O"))
    {
        lightController_->ledOff();
    }
    else if (line.endsWith("GET /W"))
    {
        lightController_->ledWork();
    }
    else if (line.endsWith("GET /R"))
    {
        lightController_->ledRest();
    }
    else if (line.endsWith("GET /B"))
    {
        lightController_->changeBrightness(0x0F);
    }
    else if (line.endsWith("GET /D"))
    {
        lightController_->changeBrightness(-0x0F);
    }
}