#include <Arduino.h>
#include "plant.h"
#include "io.h"

unsigned long currentTime = 0;

void setup() {

  Serial.begin(9600); // open the serial port at 9600 bps:
  while (!Serial) ;



  eepromTest();

  loadConfig();
  currentTime = millis();
}

void loop() {

  return;

  delay(10);
  Serial.println(millis());
  currentTime = millis();

  if(handleBluetoothConnection()) 
  {
    saveConfig();
  }

  if(isLogReady())
  {
    ringBufferId = (ringBufferId + 1) % logHistory; //increase buffer pointer

    for(auto &p: plants)
    {
      p.logHydration(getHydrationFromPlant(p));
    }
  }

  for(auto &p: plants)
  {

    if(p.wateringmode == TIME && p.isWaterTimeReady())
    {
      waterPlant(p);
    }
    if(p.wateringmode == HYDRATION && p.isHydrationReady() && (millis() - p.lastwaterd) > 10*60*1000)
    {
      waterPlant(p);
    }

  }

}