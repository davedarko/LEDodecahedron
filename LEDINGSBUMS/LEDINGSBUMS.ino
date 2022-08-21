#include <SPI.h>
#include <Adafruit_DotStar.h>
#include "LedControl.h"


LedControl lc=LedControl(16,15,10,4);
unsigned long delaytime=50;

#define NUMPIXELS 64 // Number of LEDs in strip

// Here's how to control the LEDs from any two pins:
#define DATAPIN    4
#define CLOCKPIN   5
Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);

int      head  = 0, tail = -10; // Index of first 'on' and 'off' pixels
uint32_t color = 0x220000;      // 'On' color (starts red)


void setup() {

  
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP


  //we have already set the number of devices when we created the LedControl
  int devices=lc.getDeviceCount();
  //we have to init all devices in a loop
  for(int address=0;address<devices;address++) {
    /*The MAX72XX is in power-saving mode on startup*/
    lc.shutdown(address,false);
    /* Set the brightness to a medium values */
    lc.setIntensity(address,15);
    /* and clear the display */
    lc.clearDisplay(address);
  }
}

void loop() { 
  //read the number cascaded devices
  int devices=lc.getDeviceCount();
  
  //we have to init all devices in a loop
  for(int row=0;row<8;row++) {
    for(int col=0;col<8;col++) {
      for(int address=0;address<devices;address++) {
        lc.setLed(address,row,col,true);
        
        strip.setPixelColor(head, color); // 'On' pixel at head
        strip.setPixelColor(tail, 0);     // 'Off' pixel at tail
        strip.show();                     // Refresh strip
      
        if(++head >= NUMPIXELS) {         // Increment head index.  Off end of strip?
          head = 0;                       //  Yes, reset head index to start
          if((color >>= 8) == 0)          //  Next color (R->G->B) ... past blue now?
            color = 0x220000;             //   Yes, reset to red
        }
        if(++tail >= NUMPIXELS) tail = 0;

                delay(delaytime);

      }
    }
  }
    for(int address=0;address<4;address++) {
      lc.clearDisplay(address);

    }

}
