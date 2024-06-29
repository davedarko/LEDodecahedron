#include "Arduino_is31fl3733.h"

Arduino_is31fl3733 disp0(ADDR_GND, ADDR_GND);
Arduino_is31fl3733 disp1(ADDR_GND, ADDR_SCL);
Arduino_is31fl3733 disp2(ADDR_GND, ADDR_SDA);
Arduino_is31fl3733 disp3(ADDR_GND, ADDR_VCC);

Arduino_is31fl3733 disp4(ADDR_SCL, ADDR_GND);
Arduino_is31fl3733 disp5(ADDR_SCL, ADDR_SCL);
Arduino_is31fl3733 disp6(ADDR_SCL, ADDR_SDA);
Arduino_is31fl3733 disp7(ADDR_SCL, ADDR_VCC);

Arduino_is31fl3733 disp8(ADDR_SDA, ADDR_GND);
Arduino_is31fl3733 disp9(ADDR_SDA, ADDR_SCL);
Arduino_is31fl3733 disp10(ADDR_SDA, ADDR_SDA);
Arduino_is31fl3733 disp11(ADDR_SDA, ADDR_VCC);

 Arduino_is31fl3733 disp12(ADDR_VCC, ADDR_GND);
Arduino_is31fl3733 disp13(ADDR_VCC, ADDR_SCL);
Arduino_is31fl3733 disp14(ADDR_VCC, ADDR_SDA);
//Arduino_is31fl3733 disp14(ADDR_VCC, ADDR_VCC);

#define displays 12
Arduino_is31fl3733 disp[displays] = {

  //disp0

  disp0, disp1, disp2, disp3,  disp4,
  disp5,
  disp6, disp7, disp8,
  disp9,
  disp10,
  //disp11,
  disp13
  // disp12,
  // disp13 ,disp14
};

uint8_t frame_1[12][192];
uint8_t frame_2[12][192];

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
  if (millis() - time_ms > 1000)
  {
    Serial.print("fps: ");
    Serial.println(fps);

    fps = 0;
    time_ms = millis();
  }

  calculateNewFrame();

  for (uint8_t d = 0; d < displays; d++)
    for (uint8_t i = 0; i < 192; i++)
    {
      if (frame_1[d][i] < frame_2[d][i] && frame_1[d][i] < 255) frame_1[d][i]++;
      if (frame_1[d][i] > frame_2[d][i] && frame_1[d][i] > 0) frame_1[d][i]--;
    }

  for (uint8_t d = 0; d < displays; d++) {
    disp[d].SetPWM(frame_2[d]);
  }
  fps++;
}

void calculateNewFrame() {
  for (uint8_t d = 0; d < displays; d++)
  for (uint8_t i = 0; i < 192; i++)
  {

    uint8_t x = (i / 16) % 3;

    if (random(3) == x) {
      uint8_t newValue = (uint8_t) (frame_1[d][i] / 2 + random(255) - 127);

      frame_2[d][i] = newValue;
    }
    else {
      frame_2[d][i] = 0;
    }
  }
}
