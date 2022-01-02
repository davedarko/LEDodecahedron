#include "Arduino_is31fl3733.h"


Arduino_is31fl3733 disp0(ADDR_GND, ADDR_GND);
Arduino_is31fl3733 disp1(ADDR_GND, ADDR_SCL);
Arduino_is31fl3733 disp2(ADDR_GND, ADDR_SDA);
Arduino_is31fl3733 disp3(ADDR_GND, ADDR_VCC);
Arduino_is31fl3733 disp4(ADDR_SCL, ADDR_GND);
Arduino_is31fl3733 disp5(ADDR_SCL, ADDR_SCL);

Arduino_is31fl3733 disp[6] = {disp0, disp1, disp2, disp3, disp4, disp5 };


uint8_t rows[4][16] = {
  { 44, 43, 34, 33, 24, 23, 14, 13,  4,  3, 42, 32, 22, 12,  2,  1 },
  {  0, 47, 46, 45, 37, 36, 35, 27, 26, 25, 17, 16, 15,  7,  6,  5 },
  { 11, 10,  9,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
  { 51, 50, 49, 48, 41, 40, 39, 38, 31, 30, 29, 28, 21, 20, 19, 18 }
};

uint8_t colors[51][3] = {
  {0x88, 0x00, 0x00},
  {0x77, 0x00, 0x10},
  {0x66, 0x00, 0x20},
  {0x55, 0x00, 0x30},
  {0x44, 0x00, 0x40}, // 05
  {0x33, 0x00, 0x50},
  {0x22, 0x00, 0x60},
  {0x11, 0x00, 0x70},
  {0x00, 0x00, 0x80},
  {0x00, 0x00, 0x90}, // 10
  {0x00, 0x00, 0xA0},
  {0x00, 0x00, 0xB0},
  {0x00, 0x00, 0xC0},
  {0x00, 0x00, 0xD0},
  {0x00, 0x00, 0xE0}, // 15
  {0x00, 0x10, 0xF0},
  {0x00, 0x20, 0xE0},
  {0x00, 0x30, 0xD0},
  {0x00, 0x40, 0xC0},
  {0x00, 0x50, 0xB0}, // 20
  {0x00, 0x60, 0xA0},
  {0x00, 0x70, 0x90},
  {0x00, 0x80, 0x80},
  {0x00, 0x90, 0x70},
  {0x00, 0xA0, 0x60}, // 25
  {0x00, 0xBB, 0x50},
  {0x00, 0xCC, 0x40},
  {0x00, 0xDD, 0x30},
  {0x00, 0xEE, 0x20},
  {0x00, 0xFF, 0x10}, // 30
  {0x10, 0xEE, 0x00},
  {0x20, 0xDD, 0x00},
  {0x30, 0xCC, 0x00},
  {0x40, 0xBB, 0x00},
  {0x50, 0xAA, 0x00}, // 35
  {0x60, 0x99, 0x00},
  {0x70, 0x88, 0x00},
  {0x80, 0x77, 0x00},
  {0x90, 0x66, 0x00},
  {0xA0, 0x55, 0x00}, // 40
  {0xBB, 0x44, 0x00},
  {0xCC, 0x33, 0x00},
  {0xDD, 0x22, 0x00},
  {0xEE, 0x11, 0x00},
  {0xFF, 0x00, 0x00}, // 45
  {0xEE, 0x00, 0x00},
  {0xDD, 0x00, 0x00},
  {0xCC, 0x00, 0x00},
  {0xBB, 0x00, 0x00},
  {0xAA, 0x00, 0x00}, // 50
  {0x99, 0x00, 0x00}
};

void setup()
{
  
  randomSeed(analogRead(0));
  Wire.setClock(400000);
  Serial.println("Starting");

  for (uint8_t i = 0; i < 6; i++)
  {
    disp[i].Init();
    disp[i].SetGCC(0x40); //was F0 but capacitors are singing

    for (uint8_t i_cs = 0; i_cs < 16; i_cs++) {
      for (uint8_t i_sw = 0; i_sw < 12; i_sw++) {
        disp[i].SetLEDPWM(i_cs, i_sw, 0);
        disp[i].SetLEDState(i_cs, i_sw, IS31FL3733_LED_STATE_ON);
      }
    }
  }

}

void loop() {

  for (uint8_t i = 0; i < 6; i++)
  {
    //waiting for input
    uint8_t x = 0;

    for (uint8_t i_sw = 0; i_sw < 4; i_sw++) {
      for (uint8_t i_cs = 0; i_cs < 16; i_cs++) {
        //      uint8_t val = (uint8_t) random(0, 32);

        uint8_t led = rows[i_sw][i_cs];
        if (led > 0)
        {
          led -= 1;
          disp[i].SetLEDPWM(i_cs, i_sw * 3, colors[led][0]);
          disp[i].SetLEDPWM(i_cs, i_sw * 3 + 1, colors[led][1]);
          disp[i].SetLEDPWM(i_cs, i_sw * 3 + 2, colors[led][2]);
        }

      }
    }
  }
  delay(5000);
  for (uint8_t i = 0; i < 6; i++)
  { 
    for (uint8_t i_cs = 0; i_cs < 16; i_cs++) {
      for (uint8_t i_sw = 0; i_sw < 12; i_sw++) {
        disp[i].SetLEDPWM(i_cs, i_sw, 0);
      }
    }
  }
  delay(2000);
}
