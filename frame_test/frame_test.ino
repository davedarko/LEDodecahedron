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
Arduino_is31fl3733 disp[displays] = {

  //disp0

  disp0, disp1, disp2, disp3, // disp4,
  disp5,
  disp6, disp7, disp8,
  disp9,
  disp10,
  disp11,
  disp12,
  // disp13 ,disp14
};

uint8_t frame_off[192];
uint8_t frame_on[192];

unsigned long time_ms;
uint8_t fps = 0;


void setup()
{

  Serial.begin(115200);  // start Serial for output

  for (uint8_t i = 0; i < displays; i++)
  {
    disp[i].Init();
    disp[i].SetGCC(0x30);

    for (uint8_t i_cs = 0; i_cs < IS31FL3733_CS; i_cs++) {
      for (uint8_t i_sw = 0; i_sw < IS31FL3733_SW; i_sw++) {
        disp[i].SetLEDPWM(i_cs, i_sw, 0);
        disp[i].SetLEDState(i_cs, i_sw, IS31FL3733_LED_STATE_ON);
      }
    }
  }
  time_ms = millis();
}

void loop()
{
  if (millis() - time_ms > 999)
  {
    time_ms = millis();
    Serial.print("FPS: ");
    Serial.println(fps);
    fps = 0;
  }

  

  for (uint8_t d = 0; d < displays; d++) {
    for (uint8_t i = 0; i < 192; i++)
    {
      frame_on[i] = 0;
      if ((i / 16) % 3 == 0) frame_on[i] = (uint8_t) random(fps);
      if ((i / 16) % 3 == 2) frame_on[i] = (uint8_t) random(10-fps);
    }
    disp[d].SetPWM(frame_on);
  }
  fps++;

  for (uint8_t d = 0; d < displays; d++) {
    for (uint8_t i = 0; i < 192; i++)
    {
      frame_off[i] = 0;
      if ((i / 16) % 3 == 0) frame_off[i] = (uint8_t) random(10-fps);
      if ((i / 16) % 3 == 1) frame_off[i] = (uint8_t) random(fps);
    }
    disp[d].SetPWM(frame_off);
  }
  fps++;
}
