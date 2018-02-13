# MHZ19
Arduino library for MH-Z19/MH-Z19B CO2 sensors.

## MHZ19
This library uses software or hardware serial to communicate with sensor. Using MHZ19 you can get CO2, temperature and accuracy values (see examples).

## MHZ19PWM
With this library you can get CO2 level using single pin which supports interrupt. MHZ19PWM supports three modes:
##### MHZ_CONTINUOUS_MODE
In this mode interrupt is always attached, **getCO2** method executs without significant delay.
##### MHZ_DELAYED_MODE
The library attach interrupt only when **getCO2** methods is called and detach it after retrieving single data package. All code will be delayed up to 2000ms.
##### MHZ_ASYNC_MODE
In this mode you can request the data by calling **requestData** method (in this moment interrupt will be attached) and then check if data are ready without blocking main loop (**isDataReady** method). As in first case **getCO2** method executs without significant delay.
