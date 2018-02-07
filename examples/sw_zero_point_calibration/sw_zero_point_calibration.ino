#include <SoftwareSerial.h>
#include <MHZ19.h>

SoftwareSerial ss(17,16); //TX, RX
MHZ19 mhz(&ss);

unsigned long _time = 0;
char _command[255];
byte _idx = 0;
bool _readCommand = false;

void setup()
{
  Serial.begin(115200);
  Serial.println(F("Starting..."));

  ss.begin(9600);
}

void printCurrentValues()
{
  float k = 2.0/5.0;
  
  MHZ19_RESULT response = mhz.retrieveData();
  if (response == MHZ19_RESULT_OK)
  {
    Serial.print(F("CO2: "));
    Serial.println(mhz.getCO2());
    Serial.println(mhz.getCO2() * k);
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
  Serial.println();
}

void loop()
{
  unsigned long ms = millis();
  if (ms - _time > 15000 || _time > ms)
  {
    _time = ms;
    printCurrentValues();
  }

  while (Serial.available() > 0)
  {
    char c = Serial.read();
    if (c == 13)
    {
      continue;
    }
    else if (c == 10)
    {
      _command[_idx] = '\0';
      _readCommand = true;
      _idx = 0;
    }
    else
    {
      _command[_idx] = c;
      _idx++;
    }
  }

  if (_readCommand)
  {
    _readCommand = false;
    
    if (strcmp(_command, "calibrate") == 0)
    {
      Serial.println(F("Calibration..."));
      mhz.sendCommand(0x87);
    }
  }
}

