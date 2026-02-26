#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

#define TFT_CS         3
#define TFT_RST        4 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC         2

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Replace with your Wi-Fi credentials
const char* ssid = "";
const char* password = "";

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Use this initializer if using a 1.8" TFT screen:
  tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab

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

  // Read a file from the web server
  readWebFile("http://192.168.1.3/testingimage/output/output_1.txt", 1); // Replace with your server and file URL, 1 is the display height(i use 128x14 per file)
  readWebFile("http://192.168.1.3/testingimage/output/output_2.txt", 15);
  readWebFile("http://192.168.1.3/testingimage/output/output_3.txt", 29);
  readWebFile("http://192.168.1.3/testingimage/output/output_4.txt", 43);
  readWebFile("http://192.168.1.3/testingimage/output/output_5.txt", 57);
  readWebFile("http://192.168.1.3/testingimage/output/output_6.txt", 71);
  readWebFile("http://192.168.1.3/testingimage/output/output_7.txt", 85);
  readWebFile("http://192.168.1.3/testingimage/output/output_8.txt", 99);
  readWebFile("http://192.168.1.3/testingimage/output/output_9.txt", 113);
  readWebFile("http://192.168.1.3/testingimage/output/output_10.txt", 127);
  readWebFile("http://192.168.1.3/testingimage/output/output_11.txt", 141);
  readWebFile("http://192.168.1.3/testingimage/output/output_12.txt", 155);
}

void loop() {
  // Your main code here
}

void readWebFile(const char *url, uint16_t displayHeight) {
  HTTPClient http;
  WiFiClient wifiClient; // Create a WiFiClient object

  // Specify the URL of the file to read and pass the WiFiClient object
  http.begin(wifiClient, url);

  // Send the GET request
  int httpResponseCode = http.GET();

  if (httpResponseCode == HTTP_CODE_OK) {
    // File was successfully retrieved
    String payload = http.getString(); // Get the entire response
    //Serial.println("File contents:");
    //Serial.println(payload);

    // Parse and assign payload to a dynamically allocated uint16_t array
    const char* delimiters = ", ";
    uint16_t* kohli = nullptr;
    int count = 0;
    
    char* token = strtok(const_cast<char*>(payload.c_str()), delimiters);
    //int displayHeight = 1; // Starting height for display
    
    while (token != NULL) {
      uint16_t value = strtoul(token, NULL, 16); // Convert hex string to uint16_t
      
      // Allocate memory for the new value
      uint16_t* temp = (uint16_t*)realloc(kohli, (count + 1) * sizeof(uint16_t));
      if (temp == nullptr) {
        // Memory allocation failed
        Serial.println("Memory allocation failed");
        if (kohli != nullptr) {
          free(kohli);
        }
        break;
      }
      
      kohli = temp;
      kohli[count] = value;
      count++;
      
      token = strtok(NULL, delimiters);
      
      if (count == 128) {
        // Display the chunk of data and increment height
        tft.drawRGBBitmap(1, displayHeight, kohli, 128, 1);
        displayHeight++; // Increment height for the next chunk
        free(kohli); // Free the array
        kohli = nullptr; // Reset the pointer
        count = 0; // Reset the count
      }
    }
    
    // Free the dynamically allocated memory (if any remaining)
    if (kohli != nullptr) {
      free(kohli);
    }
  } else {
    // Failed to retrieve the file
    Serial.print("HTTP request failed with error code: ");
    Serial.println(httpResponseCode);
  }
  
  // Close the HTTP connection
  http.end();
}
