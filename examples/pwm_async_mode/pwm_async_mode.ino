#include <MHZ19PWM.h>


MHZ19PWM mhz(2, MHZ_ASYNC_MODE);
unsigned long _time = 0;
bool waitMode = false;

void setup()
{
  Serial.begin(115200);
  Serial.println(F("Starting..."));

  delay(2000);

  mhz.useLimit(5000);
}

void showValue()
{
  unsigned long start = millis();
  float co2 = mhz.getCO2();
  unsigned long duration = millis() - start;

  Serial.print(F("CO2: "));
  Serial.println(co2);
  Serial.print(F("Duration: "));
  Serial.println(duration);
  Serial.println();
}

void loop()
{
  unsigned long ms = millis();

  if (waitMode)
  {
    if (ms - _time >= 500 || ms < _time)
    {
      _time = ms;
      bool state = mhz.isDataReady();
      Serial.print(F("isDataReady: "));
      Serial.println(state);
      
      if (state)
      {
        waitMode = false;
        showValue();
      }
    }
  }
  else
  {
    if (ms - _time >= 10000 || ms < _time)
    {
      _time = ms;
      waitMode = true;
      mhz.requestData();
      Serial.println(F("Request data"));
    }
  }
}
