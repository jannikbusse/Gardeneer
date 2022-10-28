#pragma once

#include <EEPROM.h>
#include "plant.h"
#include <SoftwareSerial.h> 

SoftwareSerial MyBlue(9, 8); // RX | TX 
const uint16_t PLANT_MEM_OFFSET = 16;
// | 0- 7 byte |  8 - 15     | 16 - 152 |
// | HEADER    |  FREE SPACE | plants   |

struct header
{
    uint16_t plantMemPositions[NUM_PLANTS];

    ~header()
    {
    }
};

void readFromEEPROM(int position, int size, unsigned char* res)
{
    if(size <= 0) return;
    for(int i = 0; i < size; i++)
    {
        res[i] = EEPROM.read(position + i);
    }
}

void writeToEEPROM(int position, int size, unsigned char* ptr)
{
    if(size <= 0) return;
    for(int i = 0; i < size; i ++)
    {
        EEPROM.write(i + position, (*(ptr+  i)));
    }
}

//SETS ALL BYTES TO 0
void cleanEEPROM()
{
    for(int i = 0; i < EEPROM.length(); i ++)
    {
        EEPROM.write(i, 0);
    }
}

void eepromTest()
{
    Serial.println("started eeprom test");
}

float getHydrationFromPlant(plant &p)
{
    //TODO
    return -1;
}

int waterPlant(plant &p)
{
    Serial.print("Start watering ");
    Serial.println(p.name);
    delay(10);
    MyBlue.print("Start watering ");
    MyBlue.println(p.name);
    p.isWatering = true;
    p.startetWatering = millis();
    return 0;
}

int finishWaterPlant(plant &p)
{
    Serial.print("    Ended watering ");
    Serial.println(p.name);
    delay(10);
    MyBlue.print("    Ended watering ");
    MyBlue.println(p.name);
    p.lastwaterd = millis();
    p.isWatering = 0;
    return 0;
}

int loadConfig()
{   
    header h;
    readFromEEPROM(0, sizeof(header), (unsigned char*)&h); // read header to get plant positions
    storeStruct tmp;

    for(int i = 0; i < NUM_PLANTS; i++)
    {
        if(h.plantMemPositions[i] <= 0)
        {   
            continue; //plant is not initialized
        }
        readFromEEPROM(h.plantMemPositions[i], sizeof(storeStruct), (unsigned char*)&tmp);
        plants[i] = plant(tmp);
    }
    return 0;
}


int saveConfig()
{
    header h;
    for(int i = 0; i < NUM_PLANTS; i++)
    {
        uint16_t currentMemAdress = PLANT_MEM_OFFSET + sizeof(storeStruct)* i;
        storeStruct tmp;
        plants[i].getStoreStruct(tmp);

        writeToEEPROM(currentMemAdress, sizeof(storeStruct), (unsigned char*)&tmp);
        h.plantMemPositions[i] = currentMemAdress;
    }
    writeToEEPROM(0, sizeof(header), (unsigned char*)&h);
    return -1;
}

int updatePlant(int pinID, char name[10], mode mode, 
    unsigned long waterFreq, unsigned long waterDuration, float drynessThreshold)
{
    if(pinID > 4 || pinID < 0) return -1;

    plants[pinID].active = true;
    for(int i = 0; i < 10; i++)
    {
        plants[pinID].name[i] = name[i];
    }

    plants[pinID].wateringmode = mode;
    plants[pinID].waterFrequency = waterFreq;
    plants[pinID].wateringDuration = waterDuration;
    plants[pinID].drynessThreshold = drynessThreshold;
    plants[pinID].pinID = pinID;
    return 0;
}

int handleBluetoothConnection() //returns 1 if config has to be saved. Update plant database
{
//     return 0;
//     storeStruct  ss;
//   if (MyBlue.available()) {
//     char header = 0;
//     MyBlue.readBytes(&header, 1);
//     if(header == '~') //EQUAL TO 6F
//     {
//       Serial.println("check passed");
//       MyBlue.readBytes((unsigned char*)&ss, sizeof(storeStruct));
//       printStoreStruct(ss);
//       while(MyBlue.available()){
//         MyBlue.read(); // clear buffer
//         Serial.println("D ");

//       }
//     }
//     else{
//       while(MyBlue.available()> 0){
//         String c = MyBlue.readString(); // clear buffer
//         Serial.println(c);
//       }
//     }

//   }
}

void initBluetooth()
{
     MyBlue.begin(9600); 
  
}