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

int loadConfig(plant plants[])
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

int saveConfig(plant plants[])
{
    //TODO
    return -1;
}

int handleBluetoothConnection(plant plants[]) //returns 1 if config has to be saved. Update plant database
{
    //TODO
    if(Serial.available() > 0)  
    {

    }           

    //IF SOMETHING HAPPENED RETURN 1 SO CONFIG CAN BE SAVED
}