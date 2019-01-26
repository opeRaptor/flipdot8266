#include <Arduino.h>

class Flipdot {
    void byte_to_ascii(int byte);
    void checksum(void);
    void send (void);

    int addressIN = 1;
    int address = addressIN + 16; //For an unknown reason, the MSB part is always equal to 0x31 (1) The LSB part is equal to the address defined by the rotary switch located inside the display + 1.
    const int columns = 20;
    int rows = 14;

    int header[] = {0x2, 0x00, 0x00, 0x00, 0x00}; //[0x2, address1, address2, res1, res2]
    int footer[] = {0x3, 0x00, 0x00};

    int b1;
    int b2;
    int data[columns] = {0, 0, 0, 0, 48, 56, 440, 492, 6140, 8184, 7936, 768, 768, 1536, 768, 256, 0, 0, 0, 0}; //dino
    //String converted = "";

    int packet[5+(columns*4)+3]; //packet size header of 5 columns*2 since ascii conversion footer of 3
    int asciiData[columns*4];

}














void setup() {
    Serial.begin(4800);
}

void loop() {
    send();
    delay(1000);
    Serial.println();  //added to see packets in terminal as single line
    delay(1000);
}

void send (void) {    
    

    byte_to_ascii(address);
    header[0] = 0x2;
    header[1] = b1;
    header[2] = b2;
    
    /*for (int i=0; i<data.length(); i++) {
        String hex = String(data[i], HEX);
        converted += hex;
    }*/
    
    //columns = columns - 1; //seem to be req by the display to match rotary switch number
    if ((rows % 8) > 0) {rows = rows + (8-(rows % 8));} //creates a whole number of 8bit bytes
    //rows = rows / 8; // number of 8bit bytes
    //data = ((rows * columns) / 8); 
    
    byte_to_ascii(((rows * columns) / 8) & 0xFF); //resolution calculated here 20*14/8 =35 35 in hex is 0x23 char2 is dec 50 char3 is dec51
    header[3] = b1 ;                            // & 0xFF means casted to 8 bits
    header[4] = b2 ;

    for(int i = 0; i < columns; i++) { //turns data into two ASCII bytes
      // step 1 convert to binary
      // step 2 & 0xFF to get 16 bits
      // step 3 cut in half
      // step 4 convert each half into hex
      // step 5 convert each hex into to ascii representation, 0x30 to ascii 3 ascii 0
      
      // step 2
      //Bin binary = data[i] & 0xFF;
      
      // step 3
      //Byte byte1 = binary >> 8;
      //Byte byte2 = binary >> 16;
      
      // step 4
      //hex1 = ;
      
      // step 5
      //String hexAsAscii = String(hex, HEX);

    byte highByte = (data[i] >> 8);
    byte lowByte = (data[i] & 0xFF);

    byte_to_ascii(lowByte);
    asciiData[i*4] = b1;
    asciiData[(i*4)+1] = b2;

    byte_to_ascii(highByte);
    asciiData[(i*4)+2] = b1;
    asciiData[(i*4)+3] = b2;
 
    }
    
    checksum();

    for(int i = 0; i < 5; i++) { //header
    packet[i] = header[i];
    }

    for(int i = 0; i < (columns*4); i++) { //data
    packet[i+5] = asciiData[i];
    }

    for(int i = 0; i < 3; i++) { //footer
    packet [i +5 + (columns*4)] = footer[i];
    }

    for (int i = 0; i < (5+(columns*4)+3); i++) {
        Serial.print(char(packet[i]));
    }

}
void checksum (void){
    int sum = 0;
    int crc;

    for(int c =0; c < 5; c++) { sum += header[c];}
    for(int c =0; c < (columns*4); c++) { sum += asciiData[c];}

    //tart of text (0x02) must be removed,
    //End of text (0x03) must be added
    sum += 1;

    //Result must be casted to 8 bits
    sum = sum & 0xFF;

   //Checksum is the sum XOR 255 + 1. So, sum of all bytes + checksum equal to 0 (8 bits)
    crc =  (sum ^ 255) + 1;

    byte_to_ascii(crc);
    footer[0] = 0x3;
    footer[1] = b1;
    footer[2] = b2;
}

void byte_to_ascii(int byte) {
        /*
        Convert a byte to its ascii reprensentation.
        The transmission represent each byte by their ASCII representation.
        For example, 0x67 is reprensented by 0x36 0x37 (ascii 6 and ascii 7)
        This is not an elegant way to convert the data, and this function must
        be refactored */
        
        b1 = 0;
        b2 = 0;
        b1 = byte >> 4;
        if (b1 > 9) {b1 += 0x37;}
        else{b1 += 0x30;}

        b2 = byte % 16;
        if (b2 > 9) {b2 += 0x37;}
        else{b2 += 0x30;}
          
        /*String hex = String(byte, HEX);
  			b1 = hex[0];
  			b2 = hex[1];*/
 				//return hex;
}