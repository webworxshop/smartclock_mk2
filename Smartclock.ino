
#include "SPI.h"
#include "Time.h"
 
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
    clear7Seg();
    setTime(18, 46, 0, 8, 1, 2013);
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

void colon7Seg()
{
    digitalWrite(ss, LOW);
    SPI.transfer(0x77);
    SPI.transfer(0b00010000);
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

String formatDigits(int d)
{
    if(d < 10)
        return "0" + String(d);
    return String(d);
}

void loop() 
{
    clear7Seg();
    colon7Seg();   
    write7Seg(formatDigits(hour()) + formatDigits(minute()));
    delay(500);               // wait for a second
}
