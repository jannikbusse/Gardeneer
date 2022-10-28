#pragma once
#include <Arduino.h>

enum mode{TIME, HYDRATION};

const uint8_t NUM_PLANTS = 4;
const int LOG_HISTORY = 50; //How many log values will be buffered
const unsigned long logFrequency = 4UL * 60UL * 60UL * 1000UL; //log every 4 hours. This is time between logs!!
unsigned long lastLogged = 0;
int ringBufferId = 0;


String millisToString(unsigned long in)
{
    String res = "";

    unsigned long hours = 0;
    unsigned long minutes = 0;
    unsigned long seconds = 0;

    hours = in/(60UL * 1000UL * 60UL);
    minutes = (in - (hours * 60UL * 1000UL * 60UL))/(1000UL * 60UL);
    seconds = (in - (hours * 60UL * 1000UL * 60UL) - minutes*(1000UL * 60UL))/1000UL;
    res = String(hours) + ":" + String(minutes) + ":" + String(seconds);
    return res;
}

struct storeStruct
{
    char name[10];
    bool active = false;
    unsigned long waterFrequency = 0; //Frequency is time between watering periods here!!
    unsigned long wateringDuration = 0;
    
    float drynessThreshold = 0;

    int wateringmode = TIME;
    int pinID = 0;
    
    ~storeStruct()
    {
    }
};

struct plant
{

    bool isWatering = false;
    bool active = false;
    unsigned long lastwaterd = 0;
    unsigned long waterFrequency = 0; //Frequency is time between watering periods here!!
    unsigned long wateringDuration = 0;
    unsigned long startetWatering = 0;
    
    float drynessThreshold = 0;

    float hydrationLevels[LOG_HISTORY];

    int wateringmode = TIME;
    int pinID = 0;

    char name[10];

    bool isWaterTimeAvailable()
    {
        return (lastwaterd + waterFrequency < millis());
    }

    int logHydration(float hydration)
    {
        hydrationLevels[ringBufferId] = hydration;
        return 0;
    }

    void updateName(char sname[10])
    {
        strncpy(name, sname, 10); 
    }

    bool wateringFinished()
    {
        return startetWatering + wateringDuration < millis();
    }

    bool isHydrationReady()
    {

        //if 4 out of the last 5 values were below the threshold, its ready
        int counter = 0;
        for(int i = 0; i < 5; i ++)
        {
            if(hydrationLevels[(ringBufferId - i + LOG_HISTORY)%LOG_HISTORY] < drynessThreshold) counter ++;
        }
        if(counter >= 4)
        {
            return true;
        }
        return false;
    }

    plant()
    {
        for(int i = 0; i < LOG_HISTORY; i++)
        {
            hydrationLevels[i]= 0;
        }
        waterFrequency = 11;
        wateringmode = TIME;
        wateringDuration = 0;
        active = false;
        pinID = 0;
        drynessThreshold = 0;

    }
    plant(storeStruct ss)
    {

        for(int i = 0; i < LOG_HISTORY; i++)
        {
            hydrationLevels[i]= 0;
        }
        for(int i = 0; i < 10; i++)
        {
            name[i] = ss.name[i];
        }
        waterFrequency = ss.waterFrequency;
        wateringmode = ss.wateringmode;
        drynessThreshold = ss.drynessThreshold;
        pinID = ss.pinID;
        wateringDuration = ss.wateringDuration;
        active = ss.active;
        isWatering = false;
    }
    ~plant()
    {
    }

    int getStoreStruct(storeStruct &res)
    {
        for(int i = 0; i < 10; i++)
        {
            res.name[i] = name[i];
        }
        res.active = active;
        res.drynessThreshold = drynessThreshold;
        res.pinID = pinID;
        res.waterFrequency = waterFrequency;
        res.wateringDuration = wateringDuration;
        res.wateringmode = wateringmode;
        return 0;
    }

    void printToSerial()
    {
        Serial.println("Plant:            ");
        Serial.println(name);
        Serial.print("Active:           ");
        Serial.println(active);
        Serial.print("Drynessthreshold: ");
        Serial.println(drynessThreshold);
        Serial.print("WateringFrequency:");
        Serial.println(waterFrequency);
        //Serial.print("WateringFrequency:");
        //Serial.println(millisToString(waterFrequency));
        Serial.print("WateringDuration: ");
        Serial.println(wateringDuration);
        Serial.print("Wateringmode:     ");
        Serial.println(wateringmode);
        Serial.print("ID:               ");
        Serial.println(pinID);
        Serial.print("LastWatere:       ");
        Serial.println(lastwaterd);
        Serial.println();
    }
};

bool isLogReady()
{
    return (lastLogged + logFrequency < millis());
}

void printStoreStruct(storeStruct &s)
{
    Serial.println(s.name);
    Serial.println(s.active);
    Serial.println(s.drynessThreshold);
    Serial.println(s.waterFrequency);
    Serial.println(s.wateringDuration);
    Serial.println(s.wateringmode);
    Serial.println(s.pinID);
}


plant plants[NUM_PLANTS];