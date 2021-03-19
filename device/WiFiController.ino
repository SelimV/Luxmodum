#include "WiFiController.h"

auto emptyGet = [](AsyncWebServerRequest *request) {};
auto emptyPost = [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {};

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
        "/test",
        HTTP_POST,
        emptyGet,
        NULL,
        [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
            StaticJsonDocument<200> doc;
            if (!deserializeJson(doc, data))
            {
                const char *name = doc["name"];
                for (size_t i = 0; i < strlen(name); i++)
                {
                    Serial.write(name[i]);
                }
            };
            Serial.println();

            request->send(200); //OK
        });
    //Set the light to a colour defined in JSON {"hue":int,"saturation":int,"brightness":int}
    server_.on(
        "/colour",
        HTTP_POST,
        emptyGet,
        NULL,
        [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
            
            StaticJsonDocument<200> doc; //initialise JSON
            //parse the JSON and check for errors
            if (!deserializeJson(doc, data))
            {
                //read the values
                int hue = doc["hue"];
                int saturation = doc["saturation"];
                int brightness = doc["brightness"];
                
                //change the current colour
                lightController_->updateColour(CHSV(hue, saturation, brightness));

                //print to serial
                char msg[50];
                sprintf(msg,"(h,s,b) value updated to ( %d, %d, %d)",hue,saturation,brightness);
                Serial.println(msg);
                request->send(200);//OK
            }
            else
            {
                Serial.println("error parsing the JSON");
                request->send(400);//Bad request
            };
        });
    //start pomodoro {"workTime":int,"restTime":int,"rounds":int}
    server_.on(
        "/pomodoro",
        HTTP_POST,
        emptyGet,
        NULL,
        [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
            
            StaticJsonDocument<200> doc; //initialise JSON
            //parse the JSON and check for errors
            if (!deserializeJson(doc, data))
            {
                //read the values
                int wt = doc["workTime"];
                int rt = doc["restTime"];
                int rounds = doc["rounds"];
                
                //
                lightController_->setPomodoroTimes(wt,rt);
                //
                lightController_->startPomodoro(rounds);

                //print to serial
                char msg[100];
                sprintf(msg,"Starting pomodoro for %d rounds of %d minutes of work and %d minutes of rest",rounds,wt,rt);
                Serial.println(msg);
                request->send(200);//OK
            }
            else
            {
                Serial.println("error parsing the JSON");
                request->send(400);//Bad request
            };
        });

    //change the colour of the work mode
    server_.on(
        "/setWcolour",
        HTTP_POST,
        emptyGet,
        NULL,
        [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
            StaticJsonDocument<200> doc;
            if (!deserializeJson(doc, data))
            {
                int hue = doc["hue"];
                int saturation = doc["saturation"];
                int brightness = doc["brightness"];
                //change the colour of the work mode
                lightController_->colours.work = CHSV(hue, saturation, brightness);
                //if the mode is on, show the new colour
                if(lightController_->getMode() == work){
                    lightController_->updateColour(CHSV(hue, saturation, brightness));
                }
              request->send(200);//OK
            }
            else
            {
                Serial.println("error parsing the JSON");
                request->send(400);//Bad request
            };
        });

    //change the colour of the rest mode
    server_.on(
        "/setRcolour",
        HTTP_POST,
        emptyGet,
        NULL,
        [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
            StaticJsonDocument<200> doc;
            if (!deserializeJson(doc, data))
            {
                int hue = doc["hue"];
                int saturation = doc["saturation"];
                int brightness = doc["brightness"];
                lightController_->colours.rest = CHSV(hue, saturation, brightness);
                if(lightController_->getMode() == rest){
                    lightController_->updateColour(CHSV(hue, saturation, brightness));
                }
              request->send(200);//OK
            }
            else
            {
                Serial.println("error parsing the JSON");
                request->send(400);//Bad request
            }; 
        });
    //set lights to work mode
    server_.on(
        "/W",
        HTTP_GET,
        [this](AsyncWebServerRequest *request) {
            lightController_->stopPomodoro();
            lightController_->ledWork();
            request->send(200); //OK
        },
        NULL,
        emptyPost);
    //set lights to rest mode
    server_.on(
        "/R",
        HTTP_GET,
        [this](AsyncWebServerRequest *request) {
            lightController_->stopPomodoro();
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
