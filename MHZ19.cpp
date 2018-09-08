#include "MHZ19.h"

MHZ19::MHZ19(HardwareSerial * serial)
{
	_hs = serial;
}

MHZ19::MHZ19(SoftwareSerial * serial)
{
	_ss = serial;
}

MHZ19::~MHZ19()
{
	_hs = nullptr;
	_ss = nullptr;
}

int MHZ19::getCO2()
{
	if (_result == MHZ19_RESULT_OK)
	{
		return bytes2int(_response[2], _response[3]);
	}
	return _result;
}

int MHZ19::getMinCO2()
{
	if (_result == MHZ19_RESULT_OK)
	{
		return bytes2int(_response[6], _response[7]);
	}
	return _result;
}


int MHZ19::getTemperature()
{
	if (_result == MHZ19_RESULT_OK)
	{
		int value = static_cast<int>(_response[4]);
		return value - 40;
	}
	return _result;
}

int MHZ19::getAccuracy()
{
	if (_result == MHZ19_RESULT_OK)
	{
		int value = static_cast<int>(_response[5]);
		return value;
	}
	return _result;
}

void MHZ19::setAutoCalibration(bool mode)
{
	byte value = mode ? 0xA0 : 0x00;
	sendCommand(0x79, value, 0x00, 0x00, 0x00, 0x00);
}

MHZ19_RESULT MHZ19::setRange(MHZ19_RANGE range)
{
	switch (range) {
		  case MHZ19_RANGE_1000:
			sendCommand(0x99, 0x00, 0x00, 0x00, 0x03, 0xE8);
			break;
		  case MHZ19_RANGE_2000:
			sendCommand(0x99, 0x00, 0x00, 0x00, 0x07, 0xD0);
			break;
		  case MHZ19_RANGE_3000:
			sendCommand(0x99, 0x00, 0x00, 0x00, 0x0B, 0xB8);
			break;
		  case MHZ19_RANGE_5000:
			sendCommand(0x99, 0x00, 0x00, 0x00, 0x13, 0x88);
 			break;
		  case MHZ19_RANGE_10000:
			sendCommand(0x99, 0x00, 0x00, 0x00, 0x27, 0x10);
 			break;
		  default:
			return;
	}

	_result = receiveResponse(&_response);

	if (_response[1] != 0x99) {
		_result = MHZ19_RESULT_ERR_SB;
	}
	return _result;
}


void MHZ19::calibrateZero() {
	sendCommand(0x87, 0x00, 0x00, 0x00, 0x00, 0x00);
}

void MHZ19::calibrateSpan(int span) {
	if (span < 1000)
		return;
	
	byte low = static_cast<byte>(span / 256);
	byte high = static_cast<byte>(span % 256);
	sendCommand(0x88, low, high, 0x00, 0x00, 0x00);
}

void MHZ19::sendCommand(byte command, byte b3, byte b4, byte b5, byte b6, byte b7)
{
	byte cmd[9] = { 0xFF,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
	cmd[2] = command;
	cmd[3] = b3;
	cmd[4] = b4;
	cmd[5] = b5;
	cmd[6] = b6;
	cmd[7] = b7;
	cmd[8] = calcCRC(cmd);

	write(cmd, 9);	
}

MHZ19_RESULT MHZ19::receiveResponse(byte (*cmd)[9]) {
	memset(*cmd, 0, 9);

	if (_hs)
	{
		_hs->readBytes(*cmd, 9);
	}
	else
	{
		_ss->readBytes(*cmd, 9);
	}

	byte crc = calcCRC(*cmd);

	MHZ19_RESULT _result = MHZ19_RESULT_OK;
	if ((*cmd)[0] != 0xFF)
		_result = MHZ19_RESULT_ERR_FB;
	if ((*cmd)[8] != crc)
		_result = MHZ19_RESULT_ERR_CRC;
	
	return _result;
}


MHZ19_RESULT MHZ19::retrieveData()
{
	byte cmd[9] = { 0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79 };
	for (int i = 0; i < 9; i++) {
		_response[i] = 0;
	}

	write(cmd, 9);
	
	memset(_response, 0, 9);

	if (_hs)
	{
		_hs->readBytes(_response, 9);
	}
	else
	{
		_ss->readBytes(_response, 9);
	}

	byte crc = calcCRC(_response);

	_result = MHZ19_RESULT_OK;
	if (_response[0] != 0xFF)
		_result = MHZ19_RESULT_ERR_FB;
	if (_response[1] != 0x86)
		_result = MHZ19_RESULT_ERR_SB;
	if (_response[8] != crc)
		_result = MHZ19_RESULT_ERR_CRC;
	
	return _result;
}

void MHZ19::write(byte *data, byte len)
{
	if (_hs)
	{
		while (_hs->available()) { _hs->read(); }
		_hs->write(data, len);
	}
	else
	{
		while (_ss->available()) { _ss->read(); }
		_ss->write(data, len);
	}
}

int MHZ19::bytes2int(byte h, byte l)
{
	int high = static_cast<int>(h);
	int low = static_cast<int>(l);
	return (256 * high) + low;
}

byte MHZ19::calcCRC(byte * data)
{
	byte i;
	byte crc = 0;
	for (i = 1; i < 8; i++)
	{
		crc += data[i];
	}
	crc = 255 - crc;
	crc++;

	return crc;
}
