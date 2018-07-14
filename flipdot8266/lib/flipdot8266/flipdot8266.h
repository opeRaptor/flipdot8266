#include "Arduino.h"
#ifndef flipdot8266
#define flipdot8266

class Flipdot {

    public:
        Flipdot(int address, int Xsize, int Ysize);
        void send ();
    private:
};

#endif