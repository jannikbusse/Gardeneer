#pragma once

#include <EEPROM.h>
#include "plant.h"




void readFromEEPROM(int position, int size, unsigned char* res)
{
    if(size <= 0) return;
    

    for(int i = 0; i < size; i++)
    {
        res[size - 1 - i] = EEPROM.read(position + i);
    }
}


void writeToEEPROM(int position, int size, unsigned char* ptr)
{
    if(size <= 0) return;
    for(int i = 0; i < size; i ++)
    {
        EEPROM.write(i + position, (*(ptr+ size - 1- i)));
    }
}


void eepromTest()
{
    Serial.println("started eeprom test");

    storeStruct ss;
    ss.active = 1;
    ss.drynessThreshold = 3.1f;
    ss.pinID = 2;
    ss.waterFrequency = 1111;
    ss.wateringDuration = 9999999;
    ss.wateringmode = HYDRATION;

    //writeToEEPROM(0, sizeof(storeStruct), (unsigned char*)&ss);

    storeStruct loaded;

    readFromEEPROM(0, sizeof(storeStruct), (unsigned char*)&loaded);
    Serial.println(ss.wateringDuration);

}


float getHydrationFromPlant(plant &p)
{
    //TODO
    return -1;
}

int waterPlant(plant &p)
{
    p.lastwaterd = millis();
    //TODO
    return -1;
}

int loadConfig()
{
    //TODO
    for(int i = 0; i < 4; i ++)
    {
        plant p;
        p.pinID = i;
        p.name = "Plant";
    }
    return 0;
}

int saveConfig()
{
    //TODO
    return -1;
}

int updatePlant(int pinID, String name, mode mode, 
    unsigned long waterFreq, unsigned long waterDuration, float drynessThreshold)
{
    if(pinID > 4 || pinID < 0) return -1;

    plants[pinID].active = true;
    plants[pinID].name = name;
    plants[pinID].wateringmode = mode;
    plants[pinID].waterFrequency = waterFreq;
    plants[pinID].wateringDuration = waterDuration;
    plants[pinID].drynessThreshold = drynessThreshold;
    plants[pinID].pinID = pinID;
    return 0;
}

int handleBluetoothConnection() //returns 1 if config has to be saved. Update plant database
{
    //TODO
    if(Serial.available() > 0)  
    {

    }           

    //IF SOMETHING HAPPENED RETURN 1 SO CONFIG CAN BE SAVED
}