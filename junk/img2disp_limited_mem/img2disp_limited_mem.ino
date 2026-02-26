#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

#define TFT_CS         3
#define TFT_RST        4 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC         2


#include "bean.c"


Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);


void setup(void) {
  Serial.begin(115200);
  
  // Use this initializer if using a 1.8" TFT screen:
  tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab

  delay(500);
  tft.fillScreen(ST77XX_BLACK);
  
  
  
  
  tft.drawRGBBitmap(2, 2, bean, 37, 47);
  

  //tft.fillScreen(0x001F);

  
  
  //////////////////////////////////////////////////////////////////
}

void loop() {
}
