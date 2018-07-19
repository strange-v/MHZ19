# MHZ19
Arduino library for MH-Z19/MH-Z19B CO2 sensors.

## MHZ19
This library uses software or hardware serial to communicate with sensor. Using MHZ19 you can get CO2, temperature and accuracy values (see examples).

## MHZ19PWM
With this library, you can get CO2 level using a single pin which supports interrupt. MHZ19PWM supports three modes:
##### MHZ_CONTINUOUS_MODE
In this mode interrupt is always attached, **getCO2** method executes without significant delay.
##### MHZ_DELAYED_MODE
The library attaches interrupt only when **getCO2** method is called and detaches it after retrieving a single data package. All code will be delayed up to 2000ms.
##### MHZ_ASYNC_MODE
In this mode, you can request the data by calling **requestData** method (at this moment interrupt will be attached) and then check if data are ready without blocking the main loop (**isDataReady** method). As in the first case **getCO2** method executes without significant delay.
