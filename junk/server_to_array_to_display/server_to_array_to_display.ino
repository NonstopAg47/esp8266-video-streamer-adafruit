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
  readWebFile("http://192.168.1.3/testingimage/test.txt"); // Replace with your server and file URL

  
  

}

void loop() {
  // Your main code here
}

void readWebFile(const char *url) {
  HTTPClient http;
  WiFiClient wifiClient; // Create a WiFiClient object
  
  // Specify the URL of the file to read and pass the WiFiClient object
  http.begin(wifiClient, url);
  
  // Send the GET request
  int httpResponseCode = http.GET();
  
  if (httpResponseCode == HTTP_CODE_OK) {
    // File was successfully retrieved
    String payload = http.getString();
    Serial.println("File contents:");
    Serial.println(payload);

    // Parse and assign payload to a dynamically allocated uint16_t array
    const char* delimiters = ", ";
    uint16_t* kohli = nullptr;
    int count = 0;
    
    char* token = strtok(const_cast<char*>(payload.c_str()), delimiters);
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
    }
    
    // Print the uint16_t array
    Serial.println("kohli array:");
    for (int j = 0; j < count; j++) {
      Serial.print("0x");
      Serial.print(kohli[j], HEX);
      Serial.print(", ");
    }
    Serial.println();

    

    tft.drawRGBBitmap(1, 1, kohli, 37, 47);



    // Free the dynamically allocated memory
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


