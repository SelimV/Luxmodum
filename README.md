# Luxmodum
 
# API

The device can be controlled using HTTP requests sent to the device's local IP, which the device prints to serial port when booted. Example calls and responses are provided. 
### GET /W 
Switches the device to work mode. Changes the light to a colour suitable for working and stops ongoing pomodoro.
```rest
GET  http://192.168.1.238/W
```
### GET /R
Switches the device to rest mode. Changes the light to a colour suitable for resting and stops ongoing pomodoro.
```rest
GET  http://192.168.1.238/R
```
### GET /O
Switches the device to off mode. Turns the light off, but the device remains on and listening to requests. Stops ongoing pomodoro.
```rest
GET  http://192.168.1.238/O
```
### GET /T
Toggles between the different modes. Same as pressing the hardware button or calling /W, /R and /O in sequence. Stops ongoing pomodoro.
```rest
GET  http://192.168.1.238/T
```
### POST /test
Prints the contents of the data JSON to the serial port. Used for testing.
```rest
POST http://192.168.1.238/test
Content-Type: application/json

{
    "foo": "bar"
}
```
### POST /masterBrightness
Set **masterBrightness** to the value provided in a JSON. The value ranges between 0 and 255. This is **different** from the colour brightnesses. 
```rest
POST http://192.168.1.238/masterBrightness
Content-Type: application/json

{
    "masterBrightness": 127
}
```
### POST /colour
Display the colour provided as **hue**, **saturation** and **brightness** values. The values range between 0 and 255. Does **not** change the value saved to work and rest modes or interrupt pomodoro.
```rest
POST http://192.168.1.238/colour
Content-Type: application/json

{
    "hue": 40,
    "saturation": 255,
    "brightness": 255
}
```
### POST /colourW
Sets the colour of the work mode to the one provided as **hue**, **saturation** and **brightness** values. The values range between 0 and 255. The new colour is displayed if and only if the device is in work mode. Does **not** interrupt pomodoro.
```rest
POST http://192.168.1.238/colourW
Content-Type: application/json

{
    "hue": 60,
    "saturation": 255,
    "brightness": 255
}
```
### POST /colourR
Sets the colour of the rest mode to the one provided as **hue**, **saturation** and **brightness** values. The values range between 0 and 255. The new colour is displayed if and only if the device is in rest mode. Does **not** interrupt pomodoro.
```rest
POST http://192.168.1.238/colourR
Content-Type: application/json

{
    "hue": 0,
    "saturation": 255,
    "brightness": 255
}
```
### POST /pomodoro
Starts pomodoro with intervals of **workTime** and **restTime** seconds for a given number of **rounds**. The pomodoro starts with working mode and ends in resting mode unless it is interrupted.
```rest
POST http://192.168.1.238/pomodoro
Content-Type: application/json

{
    "workTime": 1500,
    "restTime": 300,
    "rounds": 4
}
```
The above requests don't return anything particularly interesting. Expected response:
```rest
HTTP/1.1 200 OK
Content-Length: 0
Access-Control-Allow-Origin: *
Access-Control-Allow-Headers: Content-Type, Access-Control-Allow-Headers, X-Requested-With
Connection: close
Accept-Ranges: none

```
### GET /details
Returns data JSON about the current state of the device. This includes the current **mode**, which is `0 for off`, `1 for work` or `2 for rest`. The **masterBrightness** is a value between 0 and 255 and **different** from the colour brightnesses. The **pomodoro** section includes **workTime** and **restTime** in seconds, number of **roundsLeft**, a boolean (0 or 1) of whether the pomodoro is **active** and **timeUntilSwitch** which is the number of seconds to the next mode switch. The **colours** section contains the **current** colour and the colours for **work** and **rest** modes, given as **hue**, **saturation** and **brightness** values ranging between 0 and 255. See the example for the JSON format.
```rest
GET  http://192.168.1.238/details
```
Example response:
```rest
HTTP/1.1 200 OK
Content-Length: 285
Content-Type: application/json
Access-Control-Allow-Origin: *
Access-Control-Allow-Headers: Content-Type, Access-Control-Allow-Headers, X-Requested-With
Connection: close
Accept-Ranges: none

{
  "mode": 1,
  "masterBrightness": 127,
  "pomodoro": {
    "workTime": 10,
    "restTime": 5,
    "roundsLeft": 3,
    "active": 1,
    "timeUntilSwitch": 3
  },
  "colours": {
    "work": {
      "hue": 0,
      "saturation": 0,
      "brightness": 255
    },
    "rest": {
      "hue": 192,
      "saturation": 255,
      "brightness": 255
    },
    "current": {
      "hue": 0,
      "saturation": 0,
      "brightness": 255
    }
  }
}
```
