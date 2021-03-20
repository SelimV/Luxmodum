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
            for (size_t i = 0; i < len; i++)
            {
                Serial.write(data[i]);
            }

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
                sprintf(msg, "(h,s,b) value updated to ( %d, %d, %d)", hue, saturation, brightness);
                Serial.println(msg);
                request->send(200); //OK
            }
            else
            {
                Serial.println("error parsing the JSON");
                request->send(400); //Bad request
            };
        });
    //Set the masterBrightness to a value defined in JSON {"masterBrightness":int}
    server_.on(
        "/masterBrightness",
        HTTP_POST,
        emptyGet,
        NULL,
        [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
            StaticJsonDocument<200> doc; //initialise JSON
            //parse the JSON and check for errors
            if (!deserializeJson(doc, data))
            {
                //read the values
                int masterBrightness = doc["masterBrightness"];

                //change the current colour
                lightController_->setMasterBrightness(masterBrightness);

                request->send(200); //OK
            }
            else
            {
                Serial.println("error parsing the JSON");
                request->send(400); //Bad request
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
                lightController_->setPomodoroTimes(wt, rt);
                //
                lightController_->startPomodoro(rounds);

                //print to serial
                char msg[100];
                sprintf(msg, "Starting pomodoro for %d rounds of %d minutes of work and %d minutes of rest", rounds, wt, rt);
                Serial.println(msg);
                request->send(200); //OK
            }
            else
            {
                Serial.println("error parsing the JSON");
                request->send(400); //Bad request
            };
        });
    //change the colour of the work mode
    server_.on(
        "/colourW",
        HTTP_POST,
        emptyGet,
        NULL,
        [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
            Serial.println("Trying to set work colour");
            StaticJsonDocument<200> doc;
            if (!deserializeJson(doc, data))
            {

                int hue = doc["hue"];
                int saturation = doc["saturation"];
                int brightness = doc["brightness"];
                //change the colour of the work mode
                lightController_->colours.work = CHSV(hue, saturation, brightness);
                //if the mode is on, show the new colour
                if (lightController_->getMode() == work)
                {
                    lightController_->updateColour(CHSV(hue, saturation, brightness));
                }
                request->send(200); //OK
            }
            else
            {
                Serial.println("error parsing the JSON");
                request->send(400); //Bad request
            };
        });

    //change the colour of the rest mode
    server_.on(
        "/colourR",
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
                if (lightController_->getMode() == rest)
                {
                    lightController_->updateColour(CHSV(hue, saturation, brightness));
                }
                request->send(200); //OK
            }
            else
            {
                Serial.println("error parsing the JSON");
                request->send(400); //Bad request
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
    //Turns the light off (does not turn the device off)
    server_.on(
        "/O",
        HTTP_GET,
        [this](AsyncWebServerRequest *request) {
            lightController_->stopPomodoro();
            lightController_->ledOff();
            request->send(200); //OK
        },
        NULL,
        emptyPost);
    //Toggles between the three states, same as the button
    server_.on(
        "/T",
        HTTP_GET,
        [this](AsyncWebServerRequest *request) {
            lightController_->stopPomodoro();
            lightController_->toggleState();
            request->send(200); //OK
        },
        NULL,
        emptyPost);

    //get details about mode, pomododoro information and colours
    server_.on(
        "/details",
        HTTP_GET,
        [this](AsyncWebServerRequest *request) {
            //send response
            request->send(200, "application/json", lightController_->details()); //OK
        },
        NULL,
        emptyPost);
    //respond to OPTIONS perflights, otherwise not found
    server_.onNotFound([](AsyncWebServerRequest *request) {
        if (request->method() == HTTP_OPTIONS)
        {
            Serial.println("HTTP_OPTIONS");
            request->send(204);
        }
        else
        {
            Serial.println("unknown request");
            request->send(404);
        }
    });
    //get rid of CORS errors
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type, Access-Control-Allow-Headers, X-Requested-With");

    //begin the server that listens to incoming connections
    server_.begin();
}


