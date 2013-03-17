
#include "Time.h"
#include "TimeAlarms.h"
#include "Serial7SegmentDisplay.h"
#include "SerialClient.h"
#include "PubSubClient.h"
#include "Bluetooth.h"
 
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

// local variables for sensors and program state
bool displayTime = true;
unsigned int temperature = 0, temp_acc = 0, temp_cnt = 0;
unsigned long battery = 0, batt_acc = 0, batt_cnt = 0;
unsigned long lightlevel = 0, light_acc = 0, light_cnt = 0;

// configuration stuff pushed from server
unsigned int light_threshold = 300;

// static configuration stuff
byte server[] = { 10, 0, 0, 194 }; // not actually needed for serial PTP link

// function defs
void pubSubCallback(char* topic, byte* payload, unsigned int length);

// hardware driver interface defs
//SerialClient serClient;
//PubSubClient client(server, 1883, callback, serClient);
Serial7SegmentDisplay Display;
Bluetooth bluetooth;

void pubSubCallback(char* topic, byte* payload, unsigned int length)
{
  // handle message arrived
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

void printSensors()
{
    Serial.println(lightlevel);
}

void setup()
{
    Display.begin(10);
    Display.brightness(128);
    setTime(17, 20, 0, 9, 1, 2013);

    Serial.begin(57600);
    //while(!bluetooth.beginCMD())
    //{
    //    delay(100);
    //}

    // Analog setup
    analogReference(DEFAULT);
    
    //if(client.connect("smartclock"))
    //{
    //    client.publish("/devices/" + deviceName + "/status", "online");
    //    // subscribe to topics
    //}

    // Alarm setup
    displayUpdate();
    Alarm.timerRepeat(20, displayCallback);
    Alarm.timerRepeat(5, printSensors);
}

void loop()
{
    static unsigned long last_lightlevel;

    last_lightlevel = lightlevel;
    sensorsUpdate();
    if(last_lightlevel != lightlevel)
        displayUpdate();

    Alarm.delay(100);
}
