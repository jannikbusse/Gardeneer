#include <Arduino.h>
#include "plant.h"
#include "io.h"
#include <SoftwareSerial.h> 


SoftwareSerial MyBlue(9, 8); // RX | TX 
unsigned long currentTime = 0;
int flag = 0; 

void setup() {

  Serial.begin(9600); // open the serial port at 9600 bps:
  MyBlue.begin(9600); 

  while (!Serial) ;

  Serial.println("Ready to connect\nDefualt password is 1234 or 000"); 




  loadConfig();

  for(int i = 0; i < NUM_PLANTS; i ++)
  {
    plants[i].printToSerial();
  }
  //saveConfig();
  
  currentTime = millis();
}

void loop() {
  storeStruct  ss;
  if (MyBlue.available()) {
    char header = 0;
    MyBlue.readBytes(&header, 1);
    if(header == '~') //EQUAL TO 6F
    {
      Serial.println("check passed");
      MyBlue.readBytes((unsigned char*)&ss, sizeof(storeStruct));
      printStoreStruct(ss);
      while(MyBlue.available()){
        MyBlue.read(); // clear buffer
        Serial.println("D ");

      }
    }
    else{
      while(MyBlue.available()> 0){
        String c = MyBlue.readString(); // clear buffer
        Serial.println(c);
      }
    }




  }

  //plants[1].getStoreStruct(ss);
  //strncpy(ss.name, "bluet plan", 10); 
  //ss.drynessThreshold = 9999;
  //ss.pinID = 4;
  //MyBlue.write((unsigned char*)&ss, sizeof(storeStruct));
  //MyBlue.println();
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