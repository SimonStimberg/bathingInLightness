//#define SERIAL_BUFFER_SIZE 256

#include <RBDdimmer.h>


const int bulbNum = 52;
const int dimMIN = 5;
const int dimMAX = 80;


const int outputPins[52] = {

  45,   // Lamp  1
  24,   // Lamp  2
  41,   // Lamp  3
  52,   // Lamp  4
  29,   // Lamp  5
  43,   // Lamp  6
  22,   // Lamp  7
  4,   // Lamp  8

  48,   // Lamp  9
  23,   // Lamp 10
  10,   // Lamp 11
  32,   // Lamp 12
  35,   // Lamp 13
  7,   // Lamp 14
  27,   // Lamp 15
  2,   // Lamp 16

  50,   // Lamp 17
  37,   // Lamp 18
  12,   // Lamp 19
  21,   // Lamp 20
  49,   // Lamp 21
  8,   // Lamp 22
  25,   // Lamp 23
  A10,   // Lamp 24

  30,   // Lamp 25
  15,   // Lamp 26
  6,   // Lamp 27
  19,   // Lamp 28
  3,   // Lamp 29
  42,   // Lamp 30
  16,   // Lamp 31
  11,   // Lamp 32

  36,   // Lamp 33
  31,   // Lamp 34
  9,   // Lamp 35
  26,   // Lamp 36
  51,   // Lamp 37
  44,   // Lamp 38
  14,   // Lamp 39
  13,   // Lamp 40

  34,   // Lamp 41
  5,   // Lamp 42
  40,   // Lamp 43
  28,   // Lamp 44
  39,   // Lamp 45
  18,   // Lamp 46
  17,   // Lamp 47
  46,   // Lamp 48

  20,   // Lamp 49
  47,   // Lamp 50
  38,   // Lamp 51
  33,   // Lamp 52

};



#define zerocross  1  // interrupt Pin for zero cross detection

dimmerLamp *dimmerMatrix[bulbNum];  // array to hold the actual dimmers - is filled in setup()

int intensities[52];  // holds the intensity values for each lamp


int byteCount = 0;

bool stringStart = false;
bool stringComplete = false;  // whether the string is complete



void setup() {

  Serial.begin(38400);
  while (!Serial);


  // declare serial pins clearly as digital outputs
  // otherwise they won't work
  //  pinMode(0, OUTPUT);
  pinMode(1, INPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(A10, OUTPUT);


  // create dimmers and assign output pins
  for (int i = 0; i < bulbNum; i++) {
    dimmerMatrix[i] = new dimmerLamp(outputPins[i], zerocross);
  }


  // initialize every dimmer
  for (int i = 0; i < bulbNum; i++) {
    intensities[i] = dimMIN;
    dimmerMatrix[i]->begin(NORMAL_MODE, ON);
    dimmerMatrix[i]->setPower(30);            // SET ALL TO A MEDIUM LOW BRIGHTNESS TO SEE IF ALL OBEY
  }


  Serial.println("initialization complete");

}



void loop() {


  // only run if a full command string is avaliable and its not reading a new one atm
//  if (stringComplete && !stringStart) {
//
//
//    for (int i = 0; i < bulbNum; i++) {
//
//      // map the intensities to suitable dimmer values
//      float dimVal = constrain((float)intensities[i], 0, 1000);
//      dimVal = map(dimVal, 0., 125., dimMIN, dimMAX);
//
//      // set the dimmer to dimmer value
//      dimmerMatrix[i]->setPower(dimVal);
//
//    }
//
//    stringComplete = false;
//
//  }

}



void serialEvent() {
//
//  while (Serial.available()) {
//    // get the new byte:
//    char inChar = (char)Serial.read();
//
//
//    // only check for serial data if its not processing the previous one in the loop function
//    if (!stringComplete) {
//
//
//      // if a starting byte has been detected read the upcoming bytes
//      if (stringStart) {
//
//
//        //  if the incoming byte is 126 -> means string ended
//        if (inChar == 126) {
//
//          // check if a full string is read (52 bytes) and set the stringComplete flag
//          // if not, the string is damaged therefore discard it and search for a new one
//          if (byteCount == 52) {
//            stringComplete = true;
//          }
//
//          stringStart = false;
//          byteCount = 0;
//
//        } else {
//
//          // add the incoming byte as integer value to the intensities array
//          intensities[byteCount] = (int)inChar;
//          byteCount++;
//
//        }
//
//      }
//
//      // if the incoming byte is 127 -> means new string starting
//      // hence set the stringStart flag
//      if (inChar == 127) {
//        stringStart = true;
//      }
//
//    }
//
//  }

}
