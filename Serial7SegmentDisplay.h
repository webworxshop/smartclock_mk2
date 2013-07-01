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

#ifndef __SERIAL_7SEGMENT_DISPLAY_H
#define __SERIAL_7SEGMENT_DISPLAY_H

#include "Arduino.h"

class Serial7SegmentDisplay
{
    public:
        void begin(int ss_pin);
        void write(String msg);
        void writeSpecial(int s);
        void clear(void);
        void brightness(byte b);

        const int ALLOFF     PROGMEM = 0x00;
        const int DECIMAL0   PROGMEM = 0x01;
        const int DECIMAL1   PROGMEM = 0x02;
        const int DECIMAL2   PROGMEM = 0x04;
        const int DECIMAL3   PROGMEM = 0x08;
        const int COLON      PROGMEM = 0x10;
        const int APOSTROPHE PROGMEM = 0x20;

    private:
        int SS;
};

#endif

