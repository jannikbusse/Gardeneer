#include <Arduino.h>

enum mode{TIME, HYDRATION};

static const int logHistory = 200;
static const unsigned long logFrequency = 60 * 60 * 1000; //log every hour. This is time between logs!!
static unsigned long lastLogged = 0;
static int ringBufferId = 0;


struct plant
{

    bool active = false;
    unsigned long lastwaterd = 0;
    unsigned long waterFrequency = 0; //Frequency is time between watering periods here!!
    unsigned long dryPerioStart = 0;
    unsigned long wateringDuration = 0;
    
    float drynessThreshold = 0;

    float hydrationLevels[logHistory];
    unsigned long timestamps[logHistory];

    int wateringmode = TIME;
    int pinID = 0;

    String name  = "";

    bool isWaterTimeReady()
    {
        lastwaterd + waterFrequency < millis();
    }

    int logHydration(float hydration)
    {
        hydrationLevels[ringBufferId] = hydration;
        timestamps[ringBufferId] = millis();
    }

};

bool isLogReady()
{
    unsigned long current = millis();
    if(lastLogged + logFrequency < current)
    {
        lastLogged = current;
        return true;
    }
    return false;
}