#pragma once

#include <EEPROM.h>
#include "plant.h"

const uint16_t PLANT_MEM_OFFSET = 16;
// | 0- 7 byte |  8 - 15     | 16 - 152 |
// | HEADER    |  FREE SPACE | plants   |
struct header
{
    uint16_t plantMemPositions[NUM_PLANTS];

    ~header()
    {
        delete(plantMemPositions);
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
    unsigned long dry = 0;    

    readFromEEPROM(0, sizeof(storeStruct), (unsigned char*)&loaded);
    readFromEEPROM(1, sizeof(unsigned long), (unsigned char*)&dry);
    Serial.println(dry);
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
    header h;
    readFromEEPROM(0, sizeof(header), (unsigned char*)&h); // read header to get plant positions
    storeStruct tmp;

    for(int i = 0; i < NUM_PLANTS; i++)
    {
        if(h.plantMemPositions[i] <= 0)
        {   
            plant pl; //THIS MIGHT NOT WORK SINCE THE PLANT IS ALLOCATED OUT OF SCOPE... NEEDS TESTING
            plants[i] = pl;
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