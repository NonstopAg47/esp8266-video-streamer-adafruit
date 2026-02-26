/*
// config.h contains
#pragma once

const char* ssid = "SSID";
const char* password = "PASSWORD";
const char* ipconfig = "192.168.1.3";  //check in cmd -> ipconfig  // server url
const uint16_t NumOfFiles = 52;
#define SerialDebug  //comment out this line to disable serial monitor
*/
#include "config.h"

#ifdef SerialDebug
  template<typename T>
  inline void println(T value) { Serial.println(value); }

  template<typename T>
  inline void println(T value, int format) { Serial.println(value, format); }

  template<typename T>
  inline void print(T value) { Serial.print(value); }
#else
  template<typename T>
  inline void println(T value) {}

  template<typename T>
  inline void println(T value, int format) {}

  template<typename T>
  inline void print(T value) {}
#endif

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include <SPI.h>

//pin definitions
#define TFT_CS 3   //RX
#define TFT_RST 4  //D2
#define TFT_DC 2   //D4
//SCL - D5
//SDA - D7


Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Create a WiFiClient object
HTTPClient http;
WiFiClient wifiClient;

//global allocation to avoid reallocating each time
const uint16_t width = 128;
const uint16_t linesPerChunk = 16; // read 16 lines at once
uint8_t buf[width * 2 * linesPerChunk];       // raw bytes
uint16_t lineBuffer[width * linesPerChunk];   // 16-bit colors

void DispImage(const char* baseUrl, uint8_t numFiles, uint16_t& startLine);
void readWebBin(const char* url, uint16_t& startLine);
void setup() {
  #ifdef SerialDebug
    Serial.begin(115200);
    delay(1000);
  #endif

  // Use this initializer if using a 1.8" TFT screen:
  tft.initR(INITR_GREENTAB);  // Init ST7735S chip, green tab
  tft.setSPISpeed(40000000);

  delay(500);
  tft.fillScreen(ST77XX_BLACK);
  delay(500);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    println("Connecting to WiFi...");
  }
  println("Connected to WiFi");

  uint16_t time = millis();
  // fetch and display images (single image)
  //DispImage("http://192.168.1.5/testingimage/00001", 12); // 12 files to fetch
  time = millis() - time;
  println(time, DEC);  //the duration it takes to display a single image


  print("initialising ");
  time = millis();

  uint16_t startLine = 0;
  char urlLoop[100];  // make sure this is large enough for full URL
  // fetch and display images (for movie)
  for (uint16_t i = 1; i <= NumOfFiles; i++) {  //00001 to NumOfFiles are folder names
    // Format i with leading zeros to have a fixed length of 5 digits
    char formattedI[6]; // 5 digits + null terminator
    sprintf(formattedI, "%05d", i);  // pads with zeros automatically
    sprintf(urlLoop, "http://%s/testingimage/%s", ipconfig, formattedI);
  
    //DispImage(urlLoop.c_str(), 12);   // 12 files to fetch
    startLine = 0;
    DispImage(urlLoop, 12, startLine); // 12 .bin files per image
    /*
    NOTE:
      i made it 12 files coz more than 1808 pixels being assigned to an array crashes. 
      i rounded and used 1792 pixels (128x14) and split 1 image to 12 sections
    */
    println(i);
  }
  print("done till ");
  println(NumOfFiles);

  time = millis() - time;
  print("Time taken: ");
  println(time, DEC);  //took these many ms to execute

  pinMode(LED_BUILTIN, OUTPUT);  // Initialize the LED_BUILTIN pin as an output
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);  // Turn the LED on (Note that LOW is the voltage level
  // but actually the LED is on; this is because
  // it is active low on the ESP-01)
  delay(500);                      // Wait for a second
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(500);                      // Wait for two seconds (to demonstrate the active low LED)
}


// Display a full image by fetching multiple .bin files
void DispImage(const char* baseUrl, uint8_t numFiles, uint16_t& startLine) {
  char url[120];  // adjust size as needed
  for (uint8_t i = 1; i <= numFiles; i++) {
    sprintf(url, "%s/output_%d.bin", baseUrl, i);
    readWebBin(url, startLine); // startLine updates automatically
  }
}

// Optimized function to read a .bin file and display 4 lines at a time
void readWebBin(const char* url, uint16_t& startLine) {
  http.begin(wifiClient, url);
  http.setReuse(true); 
  int httpCode = http.GET();

  if (httpCode != HTTP_CODE_OK) {
    println("HTTP request failed: " + String(httpCode) + " -> " + url);
    http.end();
    return;
  }

  int contentLength = http.getSize();  //total num of bytes in chunk //2 bytes per pixel
  if (contentLength <= 0) {
    println("Empty file: " + String(url));
    http.end();
    return;
  }

  WiFiClient* stream = http.getStreamPtr();
  uint16_t totalLines = contentLength / (2 * width);

  // Buffers to hold 4 lines at a time


  uint16_t linesRead = 0;

  while (linesRead < totalLines) {
    // Determine how many lines to read in this chunk (last chunk may be less than 4)
    uint16_t linesThisChunk = min(linesPerChunk, (uint16_t)(totalLines - linesRead));
    uint32_t bytesToRead = linesThisChunk * width * 2;
    uint32_t bytesRead = 0;
    uint32_t startTime = millis();

    // Read the chunk from the HTTP stream
    while (bytesRead < bytesToRead) {
      if (stream->available()) {
        int n = stream->readBytes(buf + bytesRead, bytesToRead - bytesRead);
        bytesRead += n;
      } else {
        if (millis() - startTime > 5000) {
          println("Stream timeout! -> " + String(url));
          http.end();
          return;
        }
        //delay(1);
      }
    }

    // Convert raw bytes to 16-bit color values
    for (uint16_t line = 0; line < linesThisChunk; line++) {
      for (uint16_t x = 0; x < width; x++) {
        //for rgb for display with black tab
        //lineBuffer[line * width + x] = (buf[(line * width + x) * 2] << 8) | buf[(line * width + x) * 2 + 1];

        //for bgr for display with green tab
        uint16_t pixel = (buf[(line * width + x) * 2] << 8) | buf[(line * width + x) * 2 + 1];
        lineBuffer[line * width + x] = ((pixel & 0x001F) << 11) | (pixel & 0x07E0) | ((pixel & 0xF800) >> 11);
      }
    }

    // Draw all lines in this chunk at once
    tft.drawRGBBitmap(0, startLine + linesRead, lineBuffer, width, linesThisChunk);

    linesRead += linesThisChunk;
  }

  startLine += totalLines; // update start line for next .bin file
  http.end();
}