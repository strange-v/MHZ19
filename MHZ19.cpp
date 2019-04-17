#include "MHZ19.h"

MHZ19::MHZ19(Stream * stream)
{
	_serial = stream;
}

MHZ19::~MHZ19()
{
	_serial = nullptr;
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
			return MHZ19_RESULT_ERR_UNKNOWN;
	}

	return receiveResponse(_response);
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

MHZ19_RESULT MHZ19::retrieveData()
{
	sendCommand(0x86);
	return receiveResponse(_response);
}

void MHZ19::sendCommand(byte command, byte b3, byte b4, byte b5, byte b6, byte b7)
{
	_cmd = command;
	byte cmd[9] = { 0xFF,0x01,command,b3,b4,b5,b6,b7,0x00 };
	cmd[8] = calcCRC(cmd);

	write(cmd, 9);	
}

MHZ19_RESULT MHZ19::receiveResponse(byte cmd[9]) {
	unsigned long time = millis();
	while (_serial->available() <= 0)
	{
		if (millis() - time >= SERIAL_TIMEOUT)
        {  
            return MHZ19_RESULT_ERR_TIMEOUT;
		}
	}

	memset(cmd, 0, 9);
	_serial->readBytes(cmd, 9);

	byte crc = calcCRC(cmd);

	_result = MHZ19_RESULT_OK;
	if (cmd[0] != 0xFF)
		_result = MHZ19_RESULT_ERR_FB;
	if (cmd[1] != _cmd)
		_result = MHZ19_RESULT_ERR_SB;
	if (cmd[8] != crc)
		_result = MHZ19_RESULT_ERR_CRC;
	
	return _result;
}

void MHZ19::write(byte data[], byte len)
{
	while (_serial->available() > 0) { _serial->read(); }
	_serial->write(data, len);
	_serial->flush();
}

int MHZ19::bytes2int(byte h, byte l)
{
	int high = static_cast<int>(h);
	int low = static_cast<int>(l);
	return (256 * high) + low;
}

byte MHZ19::calcCRC(byte data[])
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
