#pragma once
#include <Arduino.h>

enum MHZ_MODE {
	MHZ_CONTINUOUS_MODE,
	MHZ_DELAYED_MODE,
	MHZ_ASYNC_MODE
};

class MHZ19PWM
{
public:
	static MHZ19PWM * instance;

#if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)
	static ICACHE_RAM_ATTR void isr();
#else 
	static void isr();
#endif

	MHZ19PWM(byte pin, MHZ_MODE mode = MHZ_DELAYED_MODE);
	~MHZ19PWM();
	void useLimit(unsigned int value);
	float getCO2();
	void requestData();
	bool isDataReady();
private:
	MHZ_MODE _mode;
	byte _pin;
	unsigned int _limit = 5000;
	bool _attached = false;
	volatile bool _skipLow = false;
	volatile float _currentValue = 0;
	unsigned long _startHigh = 0;
	unsigned long _startLow = 0;
	float _timeHigh = 0;
	float _timeLow = 0;

	void start();
	void stop();
	void waitForData();

#if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)
	void ICACHE_RAM_ATTR isrInternal();
#else
	void isrInternal();
#endif

};