
#include "SPI.h"
 
// Slave Select on pin 13
int ss = 10;
// 7 Seg Reset pin
int rst = 9;

void clear7Seg(void);

void setup() {                
    // initialize the digital pins as outputs.
    pinMode(ss, OUTPUT);
    digitalWrite(ss, HIGH);
    // set up SPI bus
    SPI.begin();
    SPI.setBitOrder(MSBFIRST);
    SPI.setClockDivider(SPI_CLOCK_DIV64);
    SPI.setDataMode(SPI_MODE0);
    // reset 7 seg
    SPI.transfer(0);
    SPI.transfer(0);
    clear7Seg();
    clear7Seg();
}

void write7Seg(String msg)
{
    int i;
    digitalWrite(ss, LOW);
    for(i = 0; i < msg.length(); i++)
    {
        SPI.transfer(msg[i]);
    }
    digitalWrite(ss, HIGH);
}

void reset7Seg()
{
    digitalWrite(ss, LOW);
    SPI.transfer(0x81);
    digitalWrite(ss, HIGH);
}

void clear7Seg()
{
    digitalWrite(ss, LOW);
    SPI.transfer(0x76);
    digitalWrite(ss, HIGH);
}

void loop() {
    //digitalWrite(ss, HIGH);   // turn the LED on (HIGH is the voltage level)
    //delay(1000);               // wait for a second
    //digitalWrite(ss, LOW);    // turn the LED off by making the voltage LOW
 
    clear7Seg();   
    write7Seg("1234");
    delay(1000);               // wait for a second
}
