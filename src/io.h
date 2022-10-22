#pragma once

#include "plant.h"


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