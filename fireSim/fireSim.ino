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

#define displays 1 // 12

Arduino_is31fl3733 disp[displays] = {
disp12
  //disp0
// ,
  // disp0,

  /*
  disp1, disp2, disp3, // disp4,
  disp5,
  disp6, disp7, disp8,
  disp9,
  disp10,
  disp11,
  disp12,
  // disp13 ,disp14
  */
};



unsigned long time_ms;
uint8_t fps = 0;
#define canvas_height 24
#define canvas_width  29

uint8_t frameX[192];


uint8_t coolingMap[canvas_width][canvas_height];
uint8_t buffer_1[canvas_width][canvas_height];
uint8_t buffer_2[canvas_width][canvas_height];


void setup()
{

  Serial.begin(115200);  // start Serial for output

  for (uint8_t i = 0; i < displays; i++)
  {
    disp[i].Init();
    disp[i].SetGCC(0x20);

    for (uint8_t i_cs = 0; i_cs < IS31FL3733_CS; i_cs++) {
      for (uint8_t i_sw = 0; i_sw < IS31FL3733_SW; i_sw++) {
        disp[i].SetLEDPWM(i_cs, i_sw, 0);
        disp[i].SetLEDState(i_cs, i_sw, IS31FL3733_LED_STATE_ON);
      }
    }
  }
  time_ms = millis();
  feedFire();
}

void feedFire() {
  for (uint8_t x = 0; x < canvas_width; x++) {
    buffer_1[x][0] = random(127, 255);
  }
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

  displayFrame();
  fps++;
}

void displayFrame() {

  for (uint8_t y = 0; y < canvas_height; y++) {
    for (uint8_t x = 0; x < canvas_width; x++) {
      uint8_t fx = y*canvas_width+x;
      
      if (fx<192) frameX[fx] = buffer_2[x][2+y];
    }
  }
  disp[0].SetPWM(frameX);
  
}

void calculateNewFrame() {

  for (uint8_t y = 1; y < canvas_height - 1; y++) {
    for (uint8_t x = 1; x < canvas_width - 1; x++) {
      
      uint8_t n1 = buffer_1[x+1][y];
      uint8_t n2 = buffer_1[x-1][y];
      uint8_t n3 = buffer_1[x][y+1];
      uint8_t n4 = buffer_1[x][y-1];
      uint8_t c = coolingMap[x][y];

      uint16_t p = (n1+n2+n3+n4)/4;

      uint8_t p8 = 0;
      if (c < p) p8 = (uint8_t) p - c;
      buffer_2[x][y+1] = p8;
    }
  }

  for (uint8_t y = 0; y < canvas_height; y++) {
    for (uint8_t x = 0; x < canvas_width; x++) {
      buffer_1[x][y] = buffer_2[x][y];
    }
  }

  feedFire();

  // delay(200);
  shiftCoolingMap();
  
   // printFire();
  
}

void printCoolingMap() {
  for (uint8_t y = 0; y < canvas_height; y++) {
    printNumber(y);
    Serial.print(": ");
    for (uint8_t x = 0; x < canvas_width; x++) {
      printNumber(coolingMap[x][y]);
    }
    Serial.println();
  }
}

void printFire() {
  for (uint8_t y = 0; y < canvas_height; y++) {
    printNumber(y);
    Serial.print(": ");
    for (uint8_t x = 0; x < canvas_width; x++) {
      printNumber(buffer_2[x][y]);
    }
    Serial.println();
  }
}

void shiftCoolingMap() {
  for (uint8_t y = canvas_height - 1; y > 0 ; y--) {
    for (uint8_t x = 0; x < canvas_width; x++) {
      coolingMap[x][y] = coolingMap[x][y-1];
    }
  }

  for (uint8_t x = 0; x < canvas_width; x++) {
    coolingMap[x][0] = random(8);
  }
}

void printNumber(uint8_t i) {
  if (i<10) {
    Serial.print("  ");
  }
  else if (i<100) {
    Serial.print(" ");
  }
  
  Serial.print(i);
  Serial.print(" ");
}
