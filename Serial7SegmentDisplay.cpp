
#include "Arduino.h"
#include "SPI.h"
#include "Serial7SegmentDisplay.h"

void Serial7SegmentDisplay::begin(int ss_pin)
{
    SS = ss_pin;
    // initialize the digital pin as an output.
    pinMode(SS, OUTPUT);
    digitalWrite(SS, HIGH);
    // set up SPI bus
    SPI.begin();
    SPI.setBitOrder(MSBFIRST);
    SPI.setClockDivider(SPI_CLOCK_DIV64);
    SPI.setDataMode(SPI_MODE0);
    // clear 7 seg
    clear();
}

void Serial7SegmentDisplay::write(String msg)
{
    int i;
    digitalWrite(SS, LOW);
    for(i = 0; i < msg.length(); i++)
    {
        SPI.transfer(msg[i]);
    }
    digitalWrite(SS, HIGH);
}

void Serial7SegmentDisplay::writeSpecial(int s)
{
    digitalWrite(SS, LOW);
    SPI.transfer(0x77);
    SPI.transfer(s);
    digitalWrite(SS, HIGH);
}

void Serial7SegmentDisplay::clear(void)
{
    digitalWrite(SS, LOW);
    SPI.transfer(0x76);
    digitalWrite(SS, HIGH);
}        

