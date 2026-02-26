#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Replace with your Wi-Fi credentials
const char* ssid = "";
const char* password = "";

void setup() {
  Serial.begin(115200);
  delay(1000);

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
  } else {
    // Failed to retrieve the file
    Serial.print("HTTP request failed with error code: ");
    Serial.println(httpResponseCode);
  }
  
  // Close the HTTP connection
  http.end();
}
