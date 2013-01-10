
#include "Time.h"
#include "TimeAlarms.h"
#include "Serial7SegmentDisplay.h"
 
// Temperature Sensor on analog input 0
#define TEMP_SENSOR	0
// Light Sensor on analog input 1
#define LIGHT_SENSOR	1

Serial7SegmentDisplay Display;

bool displayTime = true;
unsigned int temperature = 0, temp_acc = 0, temp_cnt = 0;

String formatDigits(int d)
{
    if(d < 10)
        return "0" + String(d);
    return String(d);
}

void displayUpdate() 
{
    Serial.println("display callback");
    Display.clear();
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
    displayTime = !displayTime;
}

void sensorsUpdate()
{
    // read sensors
    float a0 = analogRead(TEMP_SENSOR);
    float a1 = analogRead(LIGHT_SENSOR);
    
    temp_acc += (((a0*3.3)/1023.0) - 0.5)*1000;
    temp_cnt++;
    if(temp_cnt >= 199)
    {
        temperature = temp_acc/temp_cnt;
        Serial.println(temp_acc);
        Serial.println(temp_cnt);
        temp_acc = 0;
        temp_cnt = 0;
    }
}

void printSensors()
{
    Serial.println(temperature);
}

void setup()
{
    Display.begin(10);
    setTime(17, 43, 0, 9, 1, 2013);

    // Analog setup
    analogReference(DEFAULT);
    Serial.begin(9600);

    // Alarm setup
    displayUpdate();
    Alarm.timerRepeat(20, displayUpdate);
    Alarm.timerRepeat(5, printSensors);
}

void loop()
{
    sensorsUpdate();
    Alarm.delay(100);
}
