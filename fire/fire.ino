/*
  USE Seeed XIAO BLE Sense - nRF52840   | mbed enabled tools
*/


#include "Arduino_is31fl3733.h"


Arduino_is31fl3733 disp0(ADDR_GND, ADDR_GND);
Arduino_is31fl3733 disp1(ADDR_GND, ADDR_SCL);
Arduino_is31fl3733 disp2(ADDR_GND, ADDR_SDA);
Arduino_is31fl3733 disp3(ADDR_GND, ADDR_VCC);

// Arduino_is31fl3733 disp4(ADDR_SCL, ADDR_GND);
Arduino_is31fl3733 disp5(ADDR_SCL, ADDR_SCL);
Arduino_is31fl3733 disp6(ADDR_SCL, ADDR_SDA);
Arduino_is31fl3733 disp7(ADDR_SCL, ADDR_VCC);

Arduino_is31fl3733 disp8(ADDR_SDA, ADDR_GND);
Arduino_is31fl3733 disp9(ADDR_SDA, ADDR_SCL);
Arduino_is31fl3733 disp10(ADDR_SDA, ADDR_SDA);
Arduino_is31fl3733 disp11(ADDR_SDA, ADDR_VCC);

Arduino_is31fl3733 disp12(ADDR_VCC, ADDR_GND);
//Arduino_is31fl3733 disp13(ADDR_VCC, ADDR_SCL);
//Arduino_is31fl3733 disp14(ADDR_VCC, ADDR_SDA);
//Arduino_is31fl3733 disp14(ADDR_VCC, ADDR_VCC);

#define displays 12
#define leds 51

Arduino_is31fl3733 disp[displays] = {
  disp0,  disp1,  disp2,  disp3,
  disp5,  disp6,  disp7,  disp8,
  disp9, disp10, disp11, disp12
};


uint8_t rows[4][16] = {
  { 44, 43, 34, 33, 24, 23, 14, 13,  4,  3, 42, 32, 22, 12,  2,  1 },
  {  0, 47, 46, 45, 37, 36, 35, 27, 26, 25, 17, 16, 15,  7,  6,  5 },
  { 11, 10,  9,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
  { 51, 50, 49, 48, 41, 40, 39, 38, 31, 30, 29, 28, 21, 20, 19, 18 }
};

uint8_t fullFrame [12][51][3];

unsigned long myTime;

int power_pin = 13;

void setup() {

  // apparently allows for stronger charging
  pinMode (power_pin, OUTPUT);
  digitalWrite(power_pin, LOW);

  initDisplays();
}


void loop() {
  // put your main code here, to run repeatedly:

generateFrame();
    drawFullFrame();
//  if (millis() - myTime > 20) {
//    generateFrame();
//    drawFullFrame();
//    myTime = millis();
//  }
  
  
}

void generateFrame() {
  for (uint8_t d=0; d<displays; d++)
  for (uint8_t l=0; l<leds; l++) {
     uint8_t r = fullFrame[d][l][0];
     uint8_t g = fullFrame[d][l][1];
     uint8_t b = fullFrame[d][l][2];
     r+=l+1;
     g=random(r/2);
     // b+=d*d+l*l;

     fullFrame[d][l][0] = r;
     fullFrame[d][l][1] = g;
     fullFrame[d][l][2] = b;
  }    
}

void initDisplays() {
  for (uint8_t i = 0; i < displays; i++)
  {
    disp[i].Init();
    // set brightness
    disp[i].SetGCC(0x28); //was F0 but capacitors are singing
    for (uint8_t i_cs = 0; i_cs < 16; i_cs++) {
      for (uint8_t i_sw = 0; i_sw < 12; i_sw++) {
        disp[i].SetLEDPWM(i_cs, i_sw, 0);
        disp[i].SetLEDState(i_cs, i_sw, IS31FL3733_LED_STATE_ON);
      }
    }
  }
}

void shutOff() {
  for (uint8_t i = 0; i < displays; i++) {
    for (uint8_t i_cs = 0; i_cs < 16; i_cs++) {
      for (uint8_t i_sw = 0; i_sw < 12; i_sw++) {
        disp[i].SetLEDPWM(i_cs, i_sw, 0);
      }
    }
  }
}

void drawFullFrame() {
  for (uint8_t i = 0; i < displays; i++) {
    for (uint8_t i_sw = 0; i_sw < 4; i_sw++) {
      for (uint8_t i_cs = 0; i_cs < 16; i_cs++) {
        uint8_t led = rows[i_sw][i_cs];
        if (led > 0) {
          led -= 1;
          disp[i].SetLEDPWM(i_cs, i_sw * 3,     fullFrame[i][led][0]);
          disp[i].SetLEDPWM(i_cs, i_sw * 3 + 1, fullFrame[i][led][1]);
          disp[i].SetLEDPWM(i_cs, i_sw * 3 + 2, fullFrame[i][led][2]);
        }
      }
    }
  }
}
