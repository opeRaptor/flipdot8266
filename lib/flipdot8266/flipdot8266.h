#ifndef flipdot8266
#define flipdot8266
#include "Arduino.h"

class Flipdot {

    public:
        Flipdot(int address, int Xsize, int Ysize);
        void send(std::vector<int> data);
    private:
        void checksum();

        int address;
        int Xsize;
        int Ysize ;
        //int packet[5+(Xsize*4)+3]; //packet size header of 5 columns*2 since ascii conversion footer of 3
        std::vector<int> packet;
};

#endif