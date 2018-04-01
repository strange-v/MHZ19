#pragma once
#include <Arduino.h>
#include <SoftwareSerial.h>

enum MHZ19_RESULT {
	MHZ19_RESULT_OK,
	MHZ19_RESULT_ERR_FB,
	MHZ19_RESULT_ERR_SB,
	MHZ19_RESULT_ERR_CRC
};

class MHZ19
{
public:
	MHZ19(HardwareSerial * serial);
	MHZ19(SoftwareSerial * serial);
	~MHZ19();
	// Call retrieveData to retrieve values from sensor and check return code
	MHZ19_RESULT retrieveData();
	int getCO2();
	int getTemperature();
	int getAccuracy();
	
	void sendCommand(byte command, byte b3 = 0, byte b4 = 0, byte b5 = 0, byte b6 = 0, byte b7 = 0);
	MHZ19_RESULT receiveResponse(byte (*cmd)[9]);	
private:
	HardwareSerial * _hs = nullptr;
	SoftwareSerial * _ss = nullptr;
	byte _response[9];
	MHZ19_RESULT _result;

	void write(byte *data, byte len);
	int bytes2int(byte h, byte l);
	byte calcCRC(byte *data);
};

