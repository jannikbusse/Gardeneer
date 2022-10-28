#include <Arduino.h>
#include "plant.h"
#include "io.h"
#include <SoftwareSerial.h> 

int flag = 0; 

void setup() {

  Serial.begin(9600); // open the serial port at 9600 bps:
    while (!Serial) ;

  initBluetooth();
  BluetoothSerial.println("Hello!");

  //Serial.println("Ready to connect\nDefualt password is 1234 or 000"); 

  loadConfig();
  strcpy(plants[0].name, "PLAN 1");
  strcpy(plants[1].name, "PLAN 2");
  strcpy(plants[2].name, "PLAN 3");
  strcpy(plants[3].name, "PLAN 4");

  plants[0].wateringDuration = 7000;
  plants[0].waterFrequency =  1000UL;
  plants[1].wateringDuration = 5000;
  plants[1].waterFrequency =  2 * 1000UL;
  plants[2].wateringDuration = 5000;
  plants[2].waterFrequency =  3 * 60UL * 1500UL;
  plants[3].wateringDuration = 5000;
  plants[3].waterFrequency =  300UL * 60UL * 1000UL;


  for(int i = 0; i < NUM_PLANTS; i ++)
  {
    plants[i].printToSerial();
  }
 
  //saveConfig();
  
}


void loop() {


  delay(10);

  if(handleBluetoothConnection()) 
  {

  }

  if(isLogReady())
  {
    logPlants();
  }
  for(int i = 0; i < NUM_PLANTS; i ++)
  {

    if(!pumpActive && !plants[i].isWatering && plants[i].wateringmode == TIME && plants[i].isWaterTimeAvailable())
    {
      waterPlant(plants[i]);
    }
    if(!pumpActive && !plants[i].isWatering && plants[i].wateringmode == HYDRATION && plants[i].isHydrationReady() && (millis() - plants[i].lastwaterd) > 10UL*60UL*1000UL)
    {
      waterPlant(plants[i]);
    }
    if(plants[i].isWatering && plants[i].wateringFinished())
    {
      finishWaterPlant(plants[i]);
    }

  }

}