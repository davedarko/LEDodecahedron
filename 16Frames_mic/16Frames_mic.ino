#include <mic.h>
#if defined(WIO_TERMINAL)
#include "processing/filters.h"
#endif

// Settings
#if defined(WIO_TERMINAL)
#define DEBUG 1                 // Enable pin pulse during ISR  
#define SAMPLES 16000*3
#elif defined(ARDUINO_ARCH_NRF52840)
#define DEBUG 1                 // Enable pin pulse during ISR  
#define SAMPLES 800
#endif

mic_config_t mic_config{
  .channel_cnt = 1,
  .sampling_rate = 16000,
  .buf_size = 1600,
#if defined(WIO_TERMINAL)
  .debug_pin = 1                // Toggles each DAC ISR (if DEBUG is set to 1)
#elif defined(ARDUINO_ARCH_NRF52840)
  .debug_pin = LED_BUILTIN                // Toggles each DAC ISR (if DEBUG is set to 1)
#endif
};

#if defined(WIO_TERMINAL)
DMA_ADC_Class Mic(&mic_config);
#elif defined(ARDUINO_ARCH_NRF52840)
NRF52840_ADC_Class Mic(&mic_config);
#endif

int16_t recording_buf[SAMPLES];
volatile uint8_t recording = 0;
volatile static bool record_ready = false;

#if defined(WIO_TERMINAL)
FilterBuHp filter;
#endif

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

// Arduino_is31fl3733 disp12(ADDR_VCC, ADDR_GND);
//Arduino_is31fl3733 disp13(ADDR_VCC, ADDR_SCL);
//Arduino_is31fl3733 disp14(ADDR_VCC, ADDR_SDA);
//Arduino_is31fl3733 disp14(ADDR_VCC, ADDR_VCC);

#define displays 12
Arduino_is31fl3733 disp[displays] = {

  //disp0

  disp0, disp1, disp2, disp3,  disp4,
  disp5,
  disp6, disp7, disp8,
  disp9,
  disp10,
  disp11,
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

  #if defined(WIO_TERMINAL)  
  pinMode(WIO_KEY_A, INPUT_PULLUP);
#endif

  Mic.set_callback(audio_rec_callback);

  if (!Mic.begin()) {
    Serial.println("Mic initialization failed");
    while (1);
  }

  Serial.println("Mic initialization done.");
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

  #if defined(WIO_TERMINAL)  
  if (digitalRead(WIO_KEY_A) == LOW && !recording) {
  
      Serial.println("Starting sampling");
      recording = 1;
      record_ready = false;  
  }
  #endif
  
  #if defined(WIO_TERMINAL)  
    if (!recording && record_ready)
  #elif defined(ARDUINO_ARCH_NRF52840)
    if (record_ready)
  #endif  
    {
      // Serial.println("Finished sampling");
      
      for (int i = 0; i < SAMPLES; i++) {
        
      //int16_t sample = filter.step(recording_buf[i]);
      int16_t sample = recording_buf[i];

      for (uint8_t d = 0; d < displays; d++)
      for (uint8_t i = 0; i < 192; i++)
      {
        // (uint8_t*) map(sample, -32767, 32767, 0, 255)
        uint8_t xyz = (uint8_t) map(sample, 0, 4000, 0, 255);
        frame_2[d][i] = xyz;
        Serial.println(xyz);
//        if (frame_1[d][i] < frame_2[d][i] && frame_1[d][i] < 255) frame_1[d][i]++;
//        if (frame_1[d][i] > frame_2[d][i] && frame_1[d][i] > 0) frame_1[d][i]--;
      }

    for (uint8_t 
      d = 0;
      d <1;
      d++
      ){
      //d < displays; d++) {
      disp[d].SetPWM(frame_2[d]);
    }
    fps++;

      
    }
    
    record_ready = false; 
    }

  calculateNewFrame();

  
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


static void audio_rec_callback(uint16_t *buf, uint32_t buf_len) {
  
  static uint32_t idx = 0;
  // Copy samples from DMA buffer to inference buffer
#if defined(WIO_TERMINAL)
  if (recording) 
#endif
  {
    for (uint32_t i = 0; i < buf_len; i++) {
  
      // Convert 12-bit unsigned ADC value to 16-bit PCM (signed) audio value
#if defined(WIO_TERMINAL)
      recording_buf[idx++] = filter.step((int16_t)(buf[i] - 1024) * 16);
      //recording_buf[idx++] = (int16_t)(buf[i] - 1024) * 16;  
#elif defined(ARDUINO_ARCH_NRF52840)
      recording_buf[idx++] = buf[i];
#endif

      if (idx >= SAMPLES){ 
      idx = 0;
      recording = 0;
      record_ready = true;
      break;
     } 
    }
  }
}
