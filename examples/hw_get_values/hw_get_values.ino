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
  MHZ19_RESULT response = mhz.retrieveData();
  if (response == MHZ19_RESULT_OK)
  {
    Serial.print(F("CO2: "));
    Serial.println(mhz.getCO2());
    Serial.print(F("Temperature: "));
    Serial.println(mhz.getTemperature());
    Serial.print(F("Accuracy: "));
    Serial.println(mhz.getAccuracy());
  }
  else
  {
    Serial.print(F("Error, code: "));
    Serial.println(response);
  }
  
  delay(15000);
}
