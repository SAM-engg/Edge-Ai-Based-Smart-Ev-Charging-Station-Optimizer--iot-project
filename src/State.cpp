#include "State.h"


String bayStatus = "FREE";
float voltage = 0.0, current = 0.0, power = 0.0, energyWh = 0.0, temperature = 0.0;

unsigned long sessionStartMs = 0;

//edge Ai variables
float predictedArrivalProb = 0.0;
int predictedDurationMin = 0;

int lastHourOfDay = 12;


String loadDecision = "Throttle";
int throttleLevel = 100;

float predictionThreshold = 0.5; // 0 to1
int peakTariffStartHr = 18; 
int peakTariffEndHr = 21;
bool overloadActive = false;
int overloadCurrentA = 16; 
int maxStationLoadW = 3000;

