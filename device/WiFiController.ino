#include "WiFiController.h"


auto emptyGet=[](AsyncWebServerRequest *request) {};
auto emptyPost=[](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {};

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
    Serial.println(WiFi.localIP()); //print the local IP

    //set the api

    //read the data in a POST request to serial
    server_.on(
        "/post",
        HTTP_POST,
        emptyGet,
        NULL,
        [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
            for (size_t i = 0; i < len; i++)
            {
                Serial.write(data[i]);
            }

            Serial.println();

            request->send(200);//OK
        });
    //set lights to work mode
    server_.on(
        "/W",
        HTTP_GET,
        [this](AsyncWebServerRequest *request) {
            lightController_->ledWork();
            request->send(200);//OK
        },
        NULL,
    emptyPost);
    //set lights to rest mode
    server_.on(
        "/R",
        HTTP_GET,
        [this](AsyncWebServerRequest *request) {
            lightController_->ledRest();
            request->send(200); //OK
        },
        NULL,
        emptyPost);
    //get the current mode of the device, colours, etc.
    server_.on(
        "/details",
        HTTP_GET,
        [this](AsyncWebServerRequest *request) {
            //send response
            request->send(200,"application/json",lightController_->details()); //OK
        },
        NULL,
        emptyPost);
    //begin the server that listens to incoming connections
    server_.begin();
}



/* void WiFiController::handleClient()
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

 */
/* void WiFiController::handleRequest(String line)
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
 */
