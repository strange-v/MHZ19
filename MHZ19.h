#pragma once
#include <Arduino.h>
#include <SoftwareSerial.h>

class MHZ19
{
public:
	MHZ19(HardwareSerial * serial);
	MHZ19(SoftwareSerial * serial);
	~MHZ19();
	int getCO2(bool useCache = false);
	int getTemperature(bool useCache = true);
	int getAccuracy(bool useCache = true);
private:
	HardwareSerial * _hs = nullptr;;
	SoftwareSerial * _ss = nullptr;;
	byte _response[9];
	int _result;

	int retrieveData();
	int bytes2int(byte h, byte l);
	byte calcResponseCRC();
};

