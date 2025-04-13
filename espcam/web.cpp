#include "web.h"
#include "cam.h"
#include "esp_wifi.h"

#define ESPNOW_WIFI_CHANNEL 6

WebServer server(80); 

void setupWiFi(const char* ssid, const char* password) {
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Force the Wi-Fi channel back to the ESP-NOW channel
  esp_wifi_set_promiscuous(true);  // Enable promiscuous mode
  esp_wifi_set_channel(ESPNOW_WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE);  // Set the channel
  esp_wifi_set_promiscuous(false);  // Disable promiscuous mode
}

void setupWebServer() {
  server.on("/", []() {
    server.send(200, "text/plain", "Hello from ESP32!");
  });

  server.on("/capture", []() {
    captureAndServeImage("capture", server); // Pass the server object
  });

  server.begin();
  Serial.println("Web server started.");
}
