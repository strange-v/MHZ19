#include <MHZ19PWM.h>


MHZ19PWM mhz(2, MHZ_DELAYED_MODE);

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
  showValue();
  delay(5000);
}

