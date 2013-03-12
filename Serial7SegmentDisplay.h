
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

