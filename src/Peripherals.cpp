#include <Arduino.h>
#include <DHT.h>
#include "State.h"
#include "Peripherals.h"
#include "config.h"


DHT   dht(DHT_PIN , DHT_TYPE);

float mapfloat(long x,long inMin,long inMax,float outMin,float outMax)
{
  return (float)(x - inMin) * (outMax - outMin) / (float)(inMax - inMin) + outMin;
}


void sample_sensor(void)
{
    int raw_current = analogRead(CURRENT_PIN); // 0 to 4095  //0 to 32A
    int raw_voltage = analogRead(VOLTAGE_PIN); // 0 to 4095 //0 to 250v

    //map voltage 0to250v
    voltage = mapfloat(raw_voltage, 0, 4095, 0.0, 250.0); // convert to voltage
    
    if(bayStatus == "CHARGING")
    {

    //map current 0to32A
    current = mapfloat(raw_current, 0, 4095, 0.0, 32.0); // convert to current
    }
    else
    {
        current = 0; // if not charging, set current to 0
    }

    //read current and 5 vakues array

    //calculate power
    power = voltage * current; 

    //to read temperature 
    float t= dht.readTemperature(DHT_PIN );
    if(!(isnan(t)))
    {
        temperature = t;
    }
   

}

float recentAvgCurrent(void)
{
    float sum = 0;
    //read recent 5 current values
    for (int i=0;i<5;i++)
    {
      sum = sum + current;
    }
    return sum / 5;
}

bool plugin_flag_once =1 ;
bool plugout_flag_once =1 ;

void plug_status(void)
{
   bool pluginReading = digitalRead(BTN_PLUGIN);
   // detect the sw is pressed
   if(pluginReading == LOW && plugin_flag_once )
   {
      sessionStartMs = millis(); // record the start time of the charging session
   // plug in switch is pressed
   plugin_flag_once = 0;
   // change bay_status FREE to charging
   if(bayStatus == "FREE")
   {
      bayStatus = "CHARGING";
      Serial.println("Bay1 plugin detecetd,Bay is charging");
      digitalWrite(RELAY_PIN, HIGH); // turn on relay 
   }
   //update leds
}

if(pluginReading == HIGH)
   {
      plugin_flag_once = 1;
   }
   // plug out switch is pressed
      bool plugoutReading = digitalRead(BTN_PLUGOUT);
   // detect the sw is pressed
   if(plugoutReading == LOW && plugout_flag_once )
   {
   // plug out switch is pressed
   plugout_flag_once = 0;
   // change bay_status charging to FREE
   if(bayStatus == "CHARGING")
   {
      bayStatus = "FREE";
      Serial.println("Bay1 plugout detected,Bay is FREE");
   }
   //update leds
}

if(plugoutReading == HIGH)
   {
      plugout_flag_once = 1;
   }
   // change bay_status  charging to FREE
   //update leds
} 

void update_led_status(void)
{
   //if bay_status is free turn on green led 
    if(bayStatus == "FREE")
    {
        digitalWrite(LED_GREEN, HIGH);
        digitalWrite(LED_YELLOW, LOW);
       
    }
    else if(bayStatus == "CHARGING")
    {
      //if bay_status is charging turn on yellow led
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_YELLOW, HIGH);
    }
   }