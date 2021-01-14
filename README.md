# MHZ19
Arduino library for MH-Z19/MH-Z19B CO2 sensors.

## MHZ19
This library uses software or hardware serial to communicate with sensor. Using MHZ19 you can get CO2, temperature and accuracy values (see examples).

### Connection
| Sensor | Arduino board |
| ------------- | ------------- |
| Vin | 5V |
| GND | GND |
| Tx | Rx |
| Rx | Tx |

When using SoftwareSerial library you should define your RX and TX pins in the constructor like this:
```cpp
SoftwareSerial ss(13, 12); //rx, tx (D7, D6) on NodeMCU ESP-12e
```
And then connect like in the table above, Rx to Tx and Tx to Rx.

## MHZ19PWM
With this library, you can get CO2 level using a single pin which supports interrupt. MHZ19PWM supports three modes:
##### MHZ_CONTINUOUS_MODE
In this mode interrupt is always attached, **getCO2** method executes without significant delay.
##### MHZ_DELAYED_MODE
The library attaches interrupt only when **getCO2** method is called and detaches it after retrieving a single data package. All code will be delayed up to 2000ms.
##### MHZ_ASYNC_MODE
In this mode, you can request the data by calling **requestData** method (at this moment interrupt will be attached) and then check if data are ready without blocking the main loop (**isDataReady** method). As in the first case **getCO2** method executes without significant delay.

## TESTED ON
Aduino Nano, Arduino MEGA, ESP8266, ESP32.

## CHANGE LOG
##### 1.1.2
Fixed bug in MHZ19PWM, now it should work on ESP8266 and ESP32

##### 1.1.1
Used include guards instead of #pragma once

##### 1.1.0
Refactored code to support both hardware and software serial in a more elegant way and more importantly, to support ESP32.

##### 1.0.0
Initial version. Contains all changes before introducing versioning.
