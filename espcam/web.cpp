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

  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(ESPNOW_WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);
}

void setupWebServer() {
  server.on("/", []() {
    String html = R"rawliteral(
      <!DOCTYPE html>
      <html>
      <head>
        <title>ESP32 Camera</title>
        <style>
          body { font-family: Arial, sans-serif; text-align: center; margin: 0; padding: 0; }
          img { max-width: 100%; height: auto; }
        </style>
      </head>
      <body>
        <h1>ESP32 Camera</h1>
        <img id="capturedImage" src="/capture" alt="Captured Image" />
        <script>
          setInterval(() => {
            const img = document.getElementById('capturedImage');
            img.src = '/capture?timestamp=' + new Date().getTime();
          }, 5000);
        </script>
      </body>
      </html>
    )rawliteral";

    server.send(200, "text/html", html);
  });

  server.on("/capture", []() {
    serveLastCapturedImage(server);
  });

  server.on("/capture/new", []() {
    captureImage();
    server.send(200, "text/plain", "New image captured.");
  });

  server.begin();
  Serial.println("Web server started.");
}