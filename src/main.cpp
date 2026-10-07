#include <Arduino.h>
#include<WiFi.h>
#include "State.h"
#include "config.h"
#include "Peripherals.h"
#include "Network.h"
#include "Telemetry.h"
#include "model.h"
#include "edge_ai.h"
#include "optimization.h"


void setup()
{
    // initialise serial monitor
    Serial.begin(115200);
    // initialise sesnor
    dht.begin();
    //config esp32 with real time
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    //config peripheral pins
    pinMode(BTN_PLUGIN, INPUT_PULLUP);
    pinMode(BTN_PLUGOUT, INPUT_PULLUP);
    pinMode(RELAY_PIN, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_RED, OUTPUT);

    //connect Board to WiFi
    connectWiFi();
    
    // Configure MQTT server
    mqtt.setServer(MQTT_SERVER, MQTT_PORT);//mqtt server addr of things and port number

    //connect board to the cloud
    connectMQTT();//token,device id

}

unsigned long now;
unsigned long last_print;

void loop()
{
    //push data every 5 seconds
    now = millis();
    if((now - last_print) > 5000)
    {
        last_print = now;
        //read data from sensors// voltage,current,temperature,power,bay satus
        sample_sensor();
          //run edge ai to get prediction
        runEdgeAIInference();

        //decide load based on predictions
        runOptimization();


        //publish the data
        publishTelemetry();

    }
    plug_status();
   updateLeds();
    
}

