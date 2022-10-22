#include <Arduino.h>
#include "plant.h"
#include "io.h"


unsigned long currentTime = 0;

void setup() {

  loadConfig(plants);
  currentTime = millis();
}

void loop() {
  delay(10);

  currentTime = millis();

  if(handleBluetoothConnection(plants)) 
  {
    saveConfig(plants);
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