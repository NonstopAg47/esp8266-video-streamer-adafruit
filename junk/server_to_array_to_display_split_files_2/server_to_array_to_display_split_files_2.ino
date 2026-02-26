#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

#define TFT_CS         3 //D8
#define TFT_RST        4 //D3        Or set to -1 and connect to Arduino RESET pin
#define TFT_DC         2 //D4
//SCL - D5
//SDA - D7


Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Wi-Fi credentials
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
  
  uint16_t time = millis();
  // fetch and display images (single image)
  //DispImage("http://192.168.1.5/testingimage/00001", 12); // 12 files to fetch 
  time = millis() - time;
  Serial.println(time, DEC);    //the duration it takes to display a single image

  
  Serial.print("initialising ");
  time = millis();
  // fetch and display images (for movie)
  for (int i = 4001; i <= 5176; i++) {      //00001 to 05176 are folder names
    // Format i with leading zeros to have a fixed length of 5 digits
    String formattedI = String(i);
    while (formattedI.length() < 5) {
        formattedI = "0" + formattedI;
    }

    String urlLoop = "http://192.168.1.5/testingimage/" + formattedI;
    DispImage(urlLoop.c_str(), 12);
    Serial.print(i);
  }
  Serial.print("done till 5176 ");
  
  time = millis() - time;
  Serial.println(time, DEC);    //took these many secs to execute
  
}

void loop() {
  // Your main code here
}

//write 1 full image
void DispImage(const char* baseUrl, int numFiles) {
  // Iterate through the specified number of files and call readWebFile for each
  for (int i = 1; i <= numFiles; i++) {
    String url = String(baseUrl) + "/output_" + String(i) + ".txt";
    int displayHeight = i * 14 - 13; // Calculate display height
    readWebFile(url.c_str(), displayHeight);     //eg: readWebFile("http://192.168.1.3/testingimage/oppenoutput/output_1.txt", displayHeight);
  }
}

//writes partial image
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
    uint16_t* kohli = nullptr;           //kohli is the array in which colors are stored
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
      
      //writes one line at a time
      if (count == 128) {                      //screen width 128
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
