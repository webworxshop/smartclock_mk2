
#include "SPI.h"
#include "Time.h"
#include "TimeAlarms.h"
 
// Slave Select on pin 13
#define SS 		10
// Temperature Sensor on analog input 0
#define TEMP_SENSOR	0
// Light Sensor on analog input 1
#define LIGHT_SENSOR	1

bool displayTime = true;
unsigned int temperature = 0, temp_acc = 0, temp_cnt = 0;

void clear7Seg(void);
void displayCallback(void);
void printSensors();

void setup() {                
    // initialize the digital pins as outputs.
    pinMode(SS, OUTPUT);
    digitalWrite(SS, HIGH);
    // set up SPI bus
    SPI.begin();
    SPI.setBitOrder(MSBFIRST);
    SPI.setClockDivider(SPI_CLOCK_DIV64);
    SPI.setDataMode(SPI_MODE0);
    // reset 7 seg
    clear7Seg();
    setTime(17, 43, 0, 9, 1, 2013);

    // Analog setup
    analogReference(DEFAULT);
    Serial.begin(9600);

    // Alarm setup
    displayCallback();
    Alarm.timerRepeat(20, displayCallback);
    Alarm.timerRepeat(5, printSensors);
}

void write7Seg(String msg)
{
    int i;
    digitalWrite(SS, LOW);
    for(i = 0; i < msg.length(); i++)
    {
        SPI.transfer(msg[i]);
    }
    digitalWrite(SS, HIGH);
}

void colon7Seg()
{
    digitalWrite(SS, LOW);
    SPI.transfer(0x77);
    SPI.transfer(0b00010000);
    digitalWrite(SS, HIGH);
}

void degrees7Seg()
{
    digitalWrite(SS, LOW);
    SPI.transfer(0x77);
    SPI.transfer(0b00100010);
    digitalWrite(SS, HIGH);
}

void reset7Seg()
{
    digitalWrite(SS, LOW);
    SPI.transfer(0x81);
    digitalWrite(SS, HIGH);
}

void clear7Seg()
{
    digitalWrite(SS, LOW);
    SPI.transfer(0x76);
    digitalWrite(SS, HIGH);
}

String formatDigits(int d)
{
    if(d < 10)
        return "0" + String(d);
    return String(d);
}

void displayCallback() 
{
    Serial.println("display callback");
    clear7Seg();
    if(displayTime)
    {
        colon7Seg();   
        write7Seg(formatDigits(hour()) + formatDigits(minute()));
    }
    else
    {
        degrees7Seg();
        write7Seg(String(temperature) + " ");
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

void loop()
{
    sensorsUpdate();
    Alarm.delay(100);
}
