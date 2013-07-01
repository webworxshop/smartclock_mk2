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

void Serial7SegmentDisplay::brightness(byte b)
{
    digitalWrite(SS, LOW);
    SPI.transfer(0x7A);
    SPI.transfer(b);
    digitalWrite(SS, HIGH);
}

