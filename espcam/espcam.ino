#include "ESP32_NOW.h"
#include "WiFi.h"

#include <esp_mac.h>

#include <vector>
#include <cstring>
#include "web.h"
#include "cam.h"

const char* ssid = "your_ssid";
const char* password = "your_password";

#define COMMAND_BUFFER_SIZE 128
char command[COMMAND_BUFFER_SIZE] = {0};

#define ESPNOW_WIFI_CHANNEL 6

class ESP_NOW_Peer_Class : public ESP_NOW_Peer {
public:
  ESP_NOW_Peer_Class(const uint8_t *mac_addr, uint8_t channel, wifi_interface_t iface, const uint8_t *lmk) : ESP_NOW_Peer(mac_addr, channel, iface, lmk) {}

  ~ESP_NOW_Peer_Class() {}

  bool add_peer() {
    if (!add()) {
      log_e("Failed to register the broadcast peer");
      return false;
    }
    return true;
  }

  void onReceive(const uint8_t *data, size_t len, bool broadcast) {
    if (len >= COMMAND_BUFFER_SIZE) {
        Serial.println("Received command is too long, ignoring.");
        return;
    }

    strncpy(command, (const char*)data, len);
    command[len] = '\0';

    Serial.printf("Received a message from master " MACSTR " (%s)\n", MAC2STR(addr()), broadcast ? "broadcast" : "unicast");
    Serial.printf("  Message: %s\n", command);

    if (strcmp(command, "capture") == 0) {
        captureImage();
        serveLastCapturedImage(server);
    }
}
};

std::vector<ESP_NOW_Peer_Class> masters;

void register_new_master(const esp_now_recv_info_t *info, const uint8_t *data, int len, void *arg) {
  if (memcmp(info->des_addr, ESP_NOW.BROADCAST_ADDR, 6) == 0) {
    Serial.printf("Unknown peer " MACSTR " sent a broadcast message\n", MAC2STR(info->src_addr));
    Serial.println("Registering the peer as a master");

    ESP_NOW_Peer_Class new_master(info->src_addr, ESPNOW_WIFI_CHANNEL, WIFI_IF_STA, NULL);

    masters.push_back(new_master);
    if (!masters.back().add_peer()) {
      Serial.println("Failed to register the new master");
      return;
    }
  } else {
    log_v("Received a unicast message from " MACSTR, MAC2STR(info->src_addr));
    log_v("Igorning the message");
  }
}

void setup() {
  Serial.begin(115200);
  setupWiFi(ssid, password);
  setupWebServer();

  WiFi.mode(WIFI_STA);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL);
  while (!WiFi.STA.started()) {
    delay(100);
  }

  Serial.println("ESP-NOW Example - Broadcast Slave");
  Serial.println("Wi-Fi parameters:");
  Serial.println("  Mode: STA");
  Serial.println("  MAC Address: " + WiFi.macAddress());
  Serial.printf("  Channel: %d\n", ESPNOW_WIFI_CHANNEL);

  if (!initCamera()) {
    Serial.println("Failed to initialize the camera");
    Serial.println("Rebooting in 5 seconds...");
    delay(5000);
    ESP.restart();
  }

  if (!ESP_NOW.begin()) {
    Serial.println("Failed to initialize ESP-NOW");
    Serial.println("Rebooting in 5 seconds...");
    delay(5000);
    ESP.restart();
  }

  ESP_NOW.onNewPeer(register_new_master, NULL);

  Serial.println("Setup complete. Waiting for a master to broadcast a message...");
  Serial.printf("Current Wi-Fi channel: %d\n", WiFi.channel());
}

void loop() {
  server.handleClient();
  delay(1000);
}