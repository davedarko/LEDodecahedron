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

//Arduino_is31fl3733 disp12(ADDR_VCC, ADDR_GND);
//Arduino_is31fl3733 disp13(ADDR_VCC, ADDR_SCL);
//Arduino_is31fl3733 disp14(ADDR_VCC, ADDR_SDA);
//Arduino_is31fl3733 disp15(ADDR_VCC, ADDR_VCC);

Arduino_is31fl3733 pentaDisplay(ADDR_VCC, ADDR_GND);

#define displays 6
Arduino_is31fl3733 disp[displays] = {
  disp0, disp1, disp2, disp3, disp4, disp5, 
  // disp6, disp7, disp8, disp9, disp10, disp11
};

unsigned long time_ms;
unsigned long frame_ms;
uint8_t fps = 0;

uint8_t frameX[192];
uint8_t multiFrameX[12][192];
uint8_t emptiness[192];

uint8_t rows[4][16] = {
  //   0   1   2 . 3 . 4 . 5 . 6   7 . 8 . 9  10  11  12  13  14  15
  { 44, 43, 34, 33, 24, 23, 14, 13,  4,  3, 42, 32, 22, 12,  2,  1 },
  {  0, 47, 46, 45, 37, 36, 35, 27, 26, 25, 17, 16, 15,  7,  6,  5 },
  { 11, 10,  9,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
  { 51, 50, 49, 48, 41, 40, 39, 38, 31, 30, 29, 28, 21, 20, 19, 18 }
};

uint8_t pixels[51][2] = {
  // 0-25
  {  0, 15}, {  0, 14}, {  0,  9}, {  0,  8}, {  1, 15}, //  1 -  5
  {  1, 14}, {  1, 13}, {  2,  3}, {  2,  2}, {  2,  1}, //  6 - 10
  {  2,  0}, {  0, 13}, {  0,  7}, {  0,  6}, {  1, 12}, // 11 - 15
  {  1, 11}, {  1, 10}, {  3, 15}, {  3, 14}, {  3, 13}, // 16 - 20
  {  3, 12}, {  0, 12}, {  0,  5}, {  0,  4}, {  1,  9}, // 21 - 25

  {  1,  8}, {  1,  7}, {  3, 11}, {  3, 10}, {  3,  9}, // 26 - 30
  {  3,  8}, {  0, 11}, {  0,  3}, {  0,  2}, {  1,  6}, // 31 - 35
  {  1,  5}, {  1,  4}, {  3,  7}, {  3,  6}, {  3,  5}, // 36 - 40
  {  3,  4}, {  0, 10}, {  0,  1}, {  0,  0}, {  1,  3}, // 41 - 45
  {  1,  2}, {  1,  1}, {  3,  3}, {  3,  2}, {  3,  1}, // 46 - 50

  {  3, 0} // 51
};

uint8_t circularPixels [51] = {
  0,
  21, 31, 41,  1, 11,
  23, 32, 33, 42, 43,  2,  3, 12, 13, 22,
  26, 34, 35, 36, 44, 45, 46,  4,  5,  6, 14, 15, 16, 24, 25,
  30, 37, 38, 39, 40, 47, 48, 49, 50,  7,  8,  9, 10, 17, 18, 19, 20, 27, 28, 29
};

uint8_t circularStages [9][10] = {
  { 1, 0, 0, 0, 0,  0, 0, 0, 0, 0},
  { 2, 12, 22, 32, 42,  0, 0, 0, 0, 0},
  { 3, 13, 23, 33, 43,  0, 0, 0, 0, 0},
  { 4, 14, 24, 34, 44,  0, 0, 0, 0, 0},
  { 5, 15, 25, 35, 45,  6, 16, 26, 36, 46},

  { 7, 17, 27, 37, 47,  0, 0, 0, 0, 0},
  { 9, 19, 29, 39, 49,  0, 0, 0, 0, 0},
  { 8, 18, 28, 38, 48, 10, 20, 30, 40, 50},
  {11, 21, 31, 41, 51,  0, 0, 0, 0, 0}
};

uint8_t colors[5][3] = {

  {127, 0, 64},  // 0 pink
  {127, 0, 0},   // 1 red
  {0, 0, 64},    // 2 blue
  {0, 0, 0},     // 3 black
  {127, 92, 127}, // 4 white
};

uint8_t kirby[51] = {
  0, 3, 1, 0, 0, //
  0, 0, 1, 0, 1, // 10
  1, 0, 0, 0, 0,
  0, 0, 1, 0, 0, // 20
  3, 0, 2, 0, 0,
  4, 0, 0, 0, 0, // 30
  0, 0, 2, 0, 4,
  0, 0, 0, 0, 0, // 40
  3, 3, 0, 0, 0,
  0, 0, 0, 0, 1, // 50
  1
};

void setup()
{
  Serial.begin(115200);  // start Serial for output

  for (uint8_t i = 0; i < displays; i++) {
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

}

void loop()
{
  if (millis() - time_ms > 1000)
  {
    Serial.print("fps: ");
    Serial.println(fps);
    Serial.println();

    fps = 0;
    time_ms = millis();
  }

//   if (millis() - frame_ms > 30) { // 200
    // calculateNewFrameStagesColors();
    // calculateNewFrameStages();
    // calculateNewFrameFillCircular();
    // drawKirby();
    // displayFrame();

//    calculateNewFrameStagesColorsMulti();
    drawKirby();
    displaySingleFrameMulti();
    
    // displayFrameMulti();
    
//    frame_ms = millis();
    fps++;
//  }
}

uint8_t row = 0;
boolean backwards = false;


void displayFrame() {
  pentaDisplay.SetPWM(frameX);
}

void displaySingleFrameMulti() {
    for (uint8_t i = 0; i < displays; i++) {
    disp[i].SetPWM(frameX);
  }
}

void displayFrameMulti() {
  for (uint8_t i = 0; i < displays; i++) {
    disp[i].SetPWM(multiFrameX[i]);
  }
}

void calculateNewFrameStagesColorsMulti() {
//  clearFrame();

 if (row == 8) backwards = true;
    if (row == 0) backwards = false;
  
    if (backwards) row--;
    else row++;

  for (uint8_t j = 0; j < displays; j++) {
    
    for (uint8_t i = 0; i < 10; i++) {
      uint8_t ledX = circularStages[row][i]; // row
      if (ledX > 0) {
        ledX = ledX - 1;
        uint8_t i_sw = pixels[ledX][0];
        uint8_t i_cs = pixels[ledX][1];
  
        if ((backwards && row != 9) || row == 8) {
          multiFrameX[j][i_cs + 16 * (i_sw * 3 + 0)] = 255;
          multiFrameX[j][i_cs + 16 * (i_sw * 3 + 1)] = 0;
          multiFrameX[j][i_cs + 16 * (i_sw * 3 + 2)] = 192;
          
        } else {
          multiFrameX[j][i_cs + 16 * (i_sw * 3 + 0)] = 255;
          multiFrameX[j][i_cs + 16 * (i_sw * 3 + 1)] = 0; //64;
          multiFrameX[j][i_cs + 16 * (i_sw * 3 + 2)] = 0;
        }
      }
    }
  }
}


void calculateNewFrameStagesColors() {
//  clearFrame();

  if (row == 8) backwards = true;
  if (row == 0) backwards = false;

  if (backwards) row--;
  else row++;

  for (uint8_t i = 0; i < 10; i++) {
    uint8_t ledX = circularStages[row][i]; // row
    if (ledX > 0) {
      ledX = ledX - 1;
      uint8_t i_sw = pixels[ledX][0];
      uint8_t i_cs = pixels[ledX][1];

      if ((backwards && row != 9) || row == 8) {
        frameX[i_cs + 16 * (i_sw * 3 + 0)] = 255;
        frameX[i_cs + 16 * (i_sw * 3 + 1)] = 0;
        frameX[i_cs + 16 * (i_sw * 3 + 2)] = 192;
        
      } else {
        frameX[i_cs + 16 * (i_sw * 3 + 0)] = 255;
        frameX[i_cs + 16 * (i_sw * 3 + 1)] = 0; //64;
        frameX[i_cs + 16 * (i_sw * 3 + 2)] = 0;
      }
    }
  }
}

void calculateNewFrameStages() {
  clearFrame();

  if (row == 8) backwards = true;
  if (row == 0) backwards = false;

  if (backwards) row--;
  else row++;

  for (uint8_t i = 0; i < 10; i++) {
    uint8_t ledX = circularStages[row][i]; // row
    if (ledX > 0) {
      ledX = ledX - 1;
      uint8_t i_sw = pixels[ledX][0];
      uint8_t i_cs = pixels[ledX][1];

      frameX[i_cs + 16 * (i_sw * 3 + 0)] = 255;
      frameX[i_cs + 16 * (i_sw * 3 + 1)] = 0;
      frameX[i_cs + 16 * (i_sw * 3 + 2)] = 192;
    }
  }
}

void drawKirby() {
  for (uint8_t i = 0; i < 51; i++) {

    uint8_t i_sw = pixels[i][0];
    uint8_t i_cs = pixels[i][1];

    frameX[i_cs + 16 * (i_sw * 3 + 0)] = colors[kirby[i]][0];
    frameX[i_cs + 16 * (i_sw * 3 + 1)] = colors[kirby[i]][1];
    frameX[i_cs + 16 * (i_sw * 3 + 2)] = colors[kirby[i]][2];
  }
}


void clearFrame() {
  for (uint8_t i = 0; i < 192; i++) {
    frameX[i] = 0;
  }
}
uint8_t linePixel = 0;
void calculateNewFrameFillCircular() {
  if (linePixel == 52) linePixel = 0;

  for (uint8_t i = 0; i < linePixel; i++) {
    uint8_t i_sw = pixels[circularPixels[i]][0];
    uint8_t i_cs = pixels[circularPixels[i]][1];

    frameX[i_cs + 16 * (i_sw * 3 + 0)] = 127;
    frameX[i_cs + 16 * (i_sw * 3 + 1)] = 32;
    frameX[i_cs + 16 * (i_sw * 3 + 2)] = 0;
  }

  for (uint8_t i = linePixel; i < 51; i++) {
    uint8_t i_sw = pixels[circularPixels[i]][0];
    uint8_t i_cs = pixels[circularPixels[i]][1];

    frameX[i_cs + 16 * (i_sw * 3 + 0)] = 127;
    frameX[i_cs + 16 * (i_sw * 3 + 1)] = 0;
    frameX[i_cs + 16 * (i_sw * 3 + 2)] = 32;
  }

  linePixel++;
}

void calculateNewFrameFill() {

  if (linePixel == 52) linePixel = 0;

  for (uint8_t i = 0; i < linePixel; i++) {
    uint8_t i_sw = pixels[i][0];
    uint8_t i_cs = pixels[i][1];

    frameX[i_cs + 16 * (i_sw * 3 + 0)] = 127;
    frameX[i_cs + 16 * (i_sw * 3 + 1)] = 32;
    frameX[i_cs + 16 * (i_sw * 3 + 2)] = 0;
  }

  for (uint8_t i = linePixel; i < 51; i++) {
    uint8_t i_sw = pixels[i][0];
    uint8_t i_cs = pixels[i][1];

    frameX[i_cs + 16 * (i_sw * 3 + 0)] = 127;
    frameX[i_cs + 16 * (i_sw * 3 + 1)] = 0;
    frameX[i_cs + 16 * (i_sw * 3 + 2)] = 32;
  }

  linePixel++;
}
void calculateNewFrameRGB() {
  uint8_t r = random(255);
  uint8_t g = random(255);
  uint8_t b = random(255);

  for (uint8_t i_sw = 0; i_sw < 4; i_sw++) {
    for (uint8_t i_cs = 0; i_cs < 16; i_cs++) {


      frameX[i_cs + 16 * (i_sw * 3 + 0)] = r;
      frameX[i_cs + 16 * (i_sw * 3 + 1)] = g;
      frameX[i_cs + 16 * (i_sw * 3 + 2)] = b;
    }
  }
}

void printNumber(uint8_t i) {
  if (i < 10) {
    Serial.print("  ");
  }
  else if (i < 100) {
    Serial.print(" ");
  }

  Serial.print(i);
  Serial.print(" ");
}
