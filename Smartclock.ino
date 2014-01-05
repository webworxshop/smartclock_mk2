/***************************************************************************
 *  Copyright 2013 Rob Connolly <rob@webworxshop.com                       *
 *                                                                         *
 *  This file is part of Smartclock MK2.                                   *
 *                                                                         *
 *  Smartclock MK2 is free software: you can redistribute it and/or modify *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  Smartclock MK2 is distributed in the hope that it will be useful,      *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with Smartclock MK2.  If not, see <http://www.gnu.org/licenses/>.*
 ***************************************************************************/

#include "Time.h"
#include "TimeAlarms.h"
#include "Serial7SegmentDisplay.h"
#include "SerialClient.h"
#include "PubSubClient.h"
#include "Bluetooth.h"
#include "Ports.h"
#include <avr/wdt.h>

//ISR(WDT_vect) { Sleepy::watchdogEvent(); }
 
// Temperature Sensor on analog input 0
#define TEMP_SENSOR	                0
// Light Sensor on analog input 1
#define LIGHT_SENSOR	            1
// Battery Sensor on analog input 2
#define BATTERY_SENSOR              2

// Number of sensor readings to average
#define TEMP_AVERAGE_READINGS       200
#define LIGHT_AVERAGE_READINGS      10
#define BATTERY_AVERAGE_READINGS    200

#define PIR         3

// local variables for sensors and program state
bool displayTime = true;
unsigned int temperature = 0, temp_acc = 0, temp_cnt = 0;
unsigned long battery = 0, batt_acc = 0, batt_cnt = 0;
unsigned long lightlevel = 0, light_acc = 0, light_cnt = 0;

// configuration stuff pushed from server
unsigned int light_threshold = 300;
char zone_root[] = "/zones/home/back_bedroom";
unsigned int pub_interval = 300;

// static configuration stuff
byte server[] = { 10, 0, 0, 194 }; // not actually needed for serial PTP link
char deviceName[] = "smartclock";

// function defs
void pubSubCallback(char* topic, byte* payload, unsigned int length);

// hardware driver interface defs
SerialClient serClient;
PubSubClient client(server, 1883, pubSubCallback, serClient);
Serial7SegmentDisplay Display;
Bluetooth bluetooth;

void pubSubCallback(char* topic, byte* payload, unsigned int length)
{
    char buf[32];
    uint32_t unixtime;
    // handle message arrived
    String t = String(topic);
    memcpy(buf, payload, length);
    buf[length] = '\0';
    if(t == "/meta/timesync/localtime")
    {
        unixtime = String(buf).toInt();
        setTime(unixtime);
    }
}

void int0_handler()
{
    char topic[64];
    sprintf(topic, "%s/sensors/motion", zone_root);
    uint8_t s = digitalRead(PIR);
    client.publish(topic, (uint8_t*)&s, sizeof(unsigned char));
}

String formatDigits(int d)
{
    if(d < 10)
        return "0" + String(d);
    return String(d);
}

void displayUpdate() 
{
    Display.clear();
    if(lightlevel >= light_threshold)
    {
        if(displayTime)
        {
            Display.writeSpecial(Display.COLON);   
            Display.write(formatDigits(hour()) + formatDigits(minute()));
        }
        else
        {
            Display.writeSpecial(Display.DECIMAL1 | Display.APOSTROPHE);
            Display.write(String(temperature) + " ");
        }
    }
    else
        Display.writeSpecial(Display.ALLOFF); 
}

void displayCallback()
{
    displayUpdate();
    displayTime = !displayTime;
}

void sensorsUpdate()
{
    // read sensors
    float a0 = analogRead(TEMP_SENSOR);
    float a1 = analogRead(LIGHT_SENSOR);
    float a2 = analogRead(BATTERY_SENSOR);

    temp_acc += (((a0*3.3)/1023.0) - 0.5)*1000;
    temp_cnt++;
    if(temp_cnt >= TEMP_AVERAGE_READINGS)
    {
        temperature = temp_acc/temp_cnt;
        temp_acc = 0;
        temp_cnt = 0;
    }

    //Serial.println(a1);
    light_acc += a1;
    //Serial.println(light_acc);
    light_cnt++;
    if(light_cnt >= LIGHT_AVERAGE_READINGS)
    {
        lightlevel = light_acc/light_cnt;
        light_acc = 0;
        light_cnt = 0;
    }

    batt_acc += ((a2*3.3)/1023.0)*2820;
    batt_cnt++;
    if(batt_cnt >= BATTERY_AVERAGE_READINGS)
    {
        battery = batt_acc/batt_cnt;
        batt_acc = 0;
        batt_cnt = 0;
    }
}

void publishSensors()
{
    char topic[64];
    char payload[8];
    
    sprintf(topic, "%s/sensors/temperature", zone_root);
    sprintf(payload, "%d", temperature);
    client.publish(topic, (uint8_t*)payload, strlen(payload));
    
    sprintf(topic, "%s/sensors/lightlevel", zone_root);
    sprintf(payload, "%d", lightlevel);
    client.publish(topic, (uint8_t*)payload, strlen(payload));
    
    sprintf(topic, "/devices/%s/metadata/battery", deviceName);
    sprintf(payload, "%d", battery);
    client.publish(topic, (uint8_t*)payload, strlen(payload));
}

void setup()
{
    char willTopic[64];
    sprintf(willTopic, "/devices/%s/status", deviceName); 
    
    pinMode(PIR, INPUT);
    attachInterrupt(1, int0_handler, CHANGE);
    
    Display.begin(10);
    Display.brightness(128);
    setTime(19, 19, 0, 16, 4, 2013);

    Serial.begin(57600);
    while(!bluetooth.beginCMD())
    {
        delay(100);
    }
    bluetooth.connect();
    //while(!bluetooth.isConnected());
    delay(100);
    Display.write("CON1");

    // Analog setup
    analogReference(DEFAULT);
   
    delay(100); 
    //serClient.write((uint8_t*)deviceName, strlen(deviceName));
    if(client.connect(deviceName, willTopic, 0, 1, "offline"))
    {
        Display.write("CON2");
        // subscribe to topics
        client.subscribe("/meta/timesync/localtime");
    
        // publish online status
        // do this here to avoid race condition with the timedaemon
        client.publish(willTopic, (uint8_t*)"online", 6, 1);
    }
    else
    {
        Display.write("FAIL");
    }
    delay(5000);

    // Alarm setup
    displayUpdate();
    Alarm.timerRepeat(20, displayCallback);
    Alarm.timerRepeat(pub_interval, publishSensors);

    wdt_enable(WDTO_8S); 
}

void loop()
{
    static unsigned long last_lightlevel;

    last_lightlevel = lightlevel;
    sensorsUpdate();
    if(last_lightlevel != lightlevel)
        displayUpdate();

    //Sleepy::loseSomeTime(100);
    Alarm.delay(100);
    client.loop();
    if(client.connected())
       wdt_reset(); 
}

