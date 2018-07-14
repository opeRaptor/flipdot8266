#include "Arduino.h"
#include "flipdot8266.h"
int byte_to_asciiLOW(int byte);
int byte_to_asciiHIGH(int byte);
void checksum(void);

int address = 1;
const int Xsize = 20;
int Ysize = 14;
int packet[5+(Xsize*4)+3]; //packet size header of 5 columns*2 since ascii conversion footer of 3
//int data[Xsize] = {0, 0, 0, 0, 48, 56, 440, 492, 6140, 8184, 7936, 768, 768, 1792, 768, 256, 0, 0, 0, 0}; //dino
//int data[Xsize] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //Empty

Flipdot::Flipdot(int address, int Xsize, int Ysize) {

    //Header format [0x2, address1 (always 0x31), address2, res1, res2]
    packet[0] = 0x2;
    packet[1] = 0x31;
    packet[2] = byte_to_asciiHIGH(address);

    if ((Ysize % 8) > 0) {
        Ysize = Ysize + (8-(Ysize % 8)); //creates a whole number of 8bit bytes
    }

    //byte_to_ascii(((rows * columns) / 8) & 0xFF); 
    packet[3] = byte_to_asciiLOW(((Xsize * Ysize)/8) & 0xFF);  // & 0xFF means casted to 8 bits
    packet[4] = byte_to_asciiHIGH(((Xsize * Ysize)/8) & 0xFF); //resolution calculated here 20*14/8 =35 35 in hex is 0x23 char2 is dec 50 char3 is dec51
   
} 

void Flipdot::send(){
    for(int i = 0; i < Xsize; i++) { //turns data into two ASCII bytes
        // step 1 convert to binary
        // step 2 & 0xFF to get 16 bits
        // step 3 cut in half
        // step 4 convert each half into hex
        // step 5 convert each hex into to ascii representation, 0x30 to ascii 3 ascii 0
        
        byte highByte = (data[i] >> 8); //cuts 16bit byte into two 8bit bytes
        byte lowByte = (data[i] & 0xFF);


        packet[i*4] = byte_to_asciiLOW(lowByte);        //convert each hex into to ascii representation, 0x30 to ascii 3 ascii 0
        packet[(i*4)+1] = byte_to_asciiHIGH(lowByte);


        packet[(i*4)+2] = byte_to_asciiLOW(highByte);
        packet[(i*4)+3] = byte_to_asciiHIGH(highByte);
    }

    checksum();

    for (int i = 0; i < (5+(Xsize*4)+3); i++) {
        Serial.print(char(packet[i]));
    }
}

int byte_to_asciiLOW(int byte) {
        /*
         Convert a byte to its ascii reprensentation.
         The transmission represent each byte by their ASCII representation.
         For example, 0x67 is reprensented by 0x36 0x37 (ascii 6 and ascii 7)
        This is not an elegant way to convert the data, and this function must
        be refactored */

        int b1 = 0;
        b1 = byte >> 4;
        if (b1 > 9) {b1 += 0x37;}
        else{b1 += 0x30;}

        return b1;
        }
int byte_to_asciiHIGH(int byte) {
        /*
         Convert a byte to its ascii reprensentation.
         The transmission represent each byte by their ASCII representation.
         For example, 0x67 is reprensented by 0x36 0x37 (ascii 6 and ascii 7)
        This is not an elegant way to convert the data, and this function must
        be refactored */

        int b2 = 0;
        b2 = byte % 16;
        if (b2 > 9) {b2 += 0x37;}
        else{b2 += 0x30;}

        return b2;
        }

void checksum (void){
    
    int sum = 0;
    int crc;

    for(int c =0; c < 5; c++) { sum += packet[c];}
    for(int c =0; c < ((Xsize*4)+5); c++) { sum += packet[c];}

    //tart of text (0x02) must be removed,
    //End of text (0x03) must be added
    sum += 1;

    //Result must be casted to 8 bits
    sum = sum & 0xFF;

   //Checksum is the sum XOR 255 + 1. So, sum of all bytes + checksum equal to 0 (8 bits)
    crc =  (sum ^ 255) + 1;

    packet[5+(Xsize*4)] = 0x3;
    packet[5+(Xsize*4)+1] = byte_to_asciiLOW(crc);
    packet[5+(Xsize*4)+2] = byte_to_asciiHIGH(crc);
}