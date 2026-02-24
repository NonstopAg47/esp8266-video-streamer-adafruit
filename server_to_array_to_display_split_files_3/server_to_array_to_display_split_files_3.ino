#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include <SPI.h>
#include "wifi_config.h"

#define TFT_CS 3   //RX
#define TFT_RST 4  //D2
#define TFT_DC 2   //D4
//SCL - D5
//SDA - D7


Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Create a WiFiClient object
HTTPClient http;
WiFiClient wifiClient;

/*
// wifi_config.h contains
#pragma once

const char* ssid = "YourSSID";
const char* password = "YourPassword";
*/

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Use this initializer if using a 1.8" TFT screen:
  tft.initR(INITR_BLACKTAB);  // Init ST7735S chip, black tab

  delay(500);
  tft.fillScreen(ST77XX_BLACK);
  delay(500);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  uint16_t time = millis();
  // fetch and display images (single image)
  //DispImage("http://192.168.1.5/testingimage/00001", 12); // 12 files to fetch
  time = millis() - time;
  Serial.println(time, DEC);  //the duration it takes to display a single image


  Serial.print("initialising ");
  time = millis();

  uint16_t startLine = 0;
  // fetch and display images (for movie)
  for (uint16_t i = 1; i <= 5176; i+=20) {  //00001 to 05176 are folder names
    // Format i with leading zeros to have a fixed length of 5 digits
    char formattedI[6]; // 5 digits + null terminator
    sprintf(formattedI, "%05d", i);  // pads with zeros automatically
    String urlLoop = String("http://192.168.1.4/testingimage/") + formattedI;
  
    //DispImage(urlLoop.c_str(), 12);   // 12 files to fetching
    startLine = 0;
    DispImage(urlLoop.c_str(), 12, startLine); // 12 .bin files per image
    Serial.println(i);
  }
  Serial.println("done till 5176 ");

  time = millis() - time;
  Serial.print("Time taken: ");
  Serial.println(time, DEC);  //took these many secs to execute

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

/*
//write 1 full image
void DispImage(const char* baseUrl, uint8_t numFiles) {
  uint16_t displayHeight = 0;
  // Iterate through the specified number of files and call readWebFile for each
  for (uint8_t i = 1; i <= numFiles; i++) {
    String url = String(baseUrl) + "/output_" + String(i) + ".txt";
    displayHeight = i * 14 - 13;          // Calculate display height
    readWebFile(url.c_str(), displayHeight);  //eg: readWebFile("http://192.168.1.3/testingimage/oppenoutput/output_1.txt", displayHeight);
  }
}

//writes partial image
void readWebFile(const char* url, uint16_t displayHeight) {
  // Specify the URL of the file to read and pass the WiFiClient object
  http.begin(wifiClient, url);

  // Send the GET request
  int16_t httpResponseCode = http.GET();

  if (httpResponseCode == HTTP_CODE_OK) {
    // File was successfully retrieved
    String payload = http.getString();  // Get the entire response        
    //Serial.println("File contents:");
    //Serial.println(payload);

    // Parse and assign payload to a dynamically allocated uint16_t array

    const char* delimiters = ", ";
    char* token = strtok(const_cast<char*>(payload.c_str()), delimiters);

    uint16_t kohli[128];  // fixed buffer for one line //kohli is the array in which colors are stored
    uint8_t count = 0;

    //char* token = strtok(const_cast<char*>(payload.c_str()), delimiters);
    //int displayHeight = 1; // Starting height for display

    while (token != NULL) {
      kohli[count++] = strtoul(token, NULL, 16);  // Convert hex string to uint16_t

      //writes one line at a time
      if (count == 128) {
        tft.drawRGBBitmap(1, displayHeight++, kohli, 128, 1);
        count = 0;  // reset for next line
      }

      token = strtok(NULL, delimiters);
    }
  } 
  else {
    Serial.println("HTTP request failed: " + String(httpResponseCode) + ", " + url);
  }

  http.end();
}
*/

// Display a full image by fetching multiple .bin files
void DispImage(const char* baseUrl, uint8_t numFiles, uint16_t& startLine) {
  for (uint8_t i = 1; i <= numFiles; i++) {
    String url = String(baseUrl) + "/output_" + String(i) + ".bin";
    readWebBin(url.c_str(), startLine); // startLine updates automatically
  }
}

// Optimized function to read a .bin file and display 4 lines at a time
void readWebBin(const char* url, uint16_t& startLine) {
  http.begin(wifiClient, url);
  int httpCode = http.GET();

  if (httpCode != HTTP_CODE_OK) {
    Serial.println("HTTP request failed: " + String(httpCode) + " -> " + url);
    http.end();
    return;
  }

  int contentLength = http.getSize();
  if (contentLength <= 0) {
    Serial.println("Empty file: " + String(url));
    http.end();
    return;
  }

  WiFiClient* stream = http.getStreamPtr();
  const uint16_t width = 128;
  const uint16_t linesPerChunk = 4; // read 4 lines at once
  uint16_t totalLines = contentLength / (2 * width);

  // Buffers to hold 4 lines at a time
  uint8_t buf[width * 2 * linesPerChunk];       // raw bytes
  uint16_t lineBuffer[width * linesPerChunk];   // 16-bit colors

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
          Serial.println("Stream timeout! -> " + String(url));
          http.end();
          return;
        }
        delay(1);
      }
    }

    // Convert raw bytes to 16-bit color values
    for (uint16_t line = 0; line < linesThisChunk; line++) {
      for (uint16_t x = 0; x < width; x++) {
        lineBuffer[line * width + x] = (buf[(line * width + x) * 2] << 8) | buf[(line * width + x) * 2 + 1];
      }
    }

    // Draw all lines in this chunk at once
    tft.drawRGBBitmap(0, startLine + linesRead, lineBuffer, width, linesThisChunk);

    linesRead += linesThisChunk;
  }

  startLine += totalLines; // update start line for next .bin file
  http.end();
}