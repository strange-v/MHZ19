#include <SoftwareSerial.h>
#include <MHZ19.h>

SoftwareSerial ss(17,16);
MHZ19 mhz(&ss);

void setup()
{
  Serial.begin(115200);
  Serial.println(F("Starting..."));

  ss.begin(9600);
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
