#include "MHZ19PWM.h"

MHZ19PWM * MHZ19PWM::instance = nullptr;

void MHZ19PWM::isr()
{
	if (MHZ19PWM::instance)
		MHZ19PWM::instance->isrInternal();
}

MHZ19PWM::MHZ19PWM(byte pin, MHZ_MODE mode)
{
	MHZ19PWM::instance = this;
	_pin = pin;
	_mode = mode;

	pinMode(_pin, INPUT);
	digitalWrite(_pin, HIGH);

	if (_mode == MHZ_CONTINUOUS_MODE)
	{
		start();
	}
}

MHZ19PWM::~MHZ19PWM()
{
	if (_attached)
		detachInterrupt(_pin);
}

void MHZ19PWM::useLimit(unsigned int value)
{
	_limit = value;
}

float MHZ19PWM::getCO2()
{
	switch (_mode)
	{
	case MHZ_DELAYED_MODE:
	{
		start();
		waitForData();
		stop();
		return _currentValue;
	}
	case MHZ_ASYNC_MODE:
	{
		stop();
		waitForData();
		return _currentValue;
	}
	case MHZ_CONTINUOUS_MODE:
	default:
		waitForData();
		return _currentValue;
	}
}

void MHZ19PWM::requestData()
{
	if (_mode != MHZ_ASYNC_MODE)
		return;

	start();
}

bool MHZ19PWM::isDataReady()
{
	return _currentValue > 0;
}

void MHZ19PWM::start()
{
	_attached = true;
	_currentValue = -1;
	_skipLow = true;
	_startHigh = 0;
	_startLow = 0;
	_timeHigh = 0;
	_timeLow = 0;

	attachInterrupt(digitalPinToInterrupt(_pin), isr, CHANGE);
}

void MHZ19PWM::stop()
{
	detachInterrupt(digitalPinToInterrupt(_pin));

	_attached = false;
}

void MHZ19PWM::waitForData()
{
	while (!isDataReady()) {}
}

void MHZ19PWM::isrInternal()
{
	int value = digitalRead(_pin);
	unsigned long ms = micros();

	if (value == HIGH)
	{
		if (_skipLow)
			_skipLow = false;

		if (_timeLow > 0 && _timeHigh > 0)
		{
			float pwm = static_cast<float>(_timeHigh - 2) / (_timeHigh + _timeLow - 4);
			_currentValue = pwm * _limit;
		}

		_startHigh = ms;
		_timeLow = _startLow > 0 ? static_cast<float>(ms - _startLow) / 1000 : 0;
	}
	else
	{
		if (_skipLow)
		{
			_skipLow = false;
			return;
		}

		_startLow = ms;
		_timeHigh = _startHigh > 0 ? static_cast<float>(ms - _startHigh) / 1000 : 0;
	}
}