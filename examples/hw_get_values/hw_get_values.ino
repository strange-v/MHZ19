#include <SoftwareSerial.h>
#include <MHZ19.h>

MHZ19 mhz(&Serial1);

void setup()
{
  Serial.begin(115200);
  Serial.println(F("Starting..."));

  Serial1.begin(9600);
}

void loop()
{
  Serial.print(F("CO2: "));
  Serial.println(mhz.getCO2());
  Serial.print(F("Temperature: "));
  Serial.println(mhz.getTemperature());
  Serial.print(F("Accuracy: "));
  Serial.println(mhz.getAccuracy());
  delay(15000);
}
