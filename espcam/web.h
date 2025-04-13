#ifndef WEB_H
#define WEB_H

#include <WiFi.h>
#include <WebServer.h>

extern WebServer server;

void setupWiFi(const char* ssid, const char* password);
void setupWebServer();

#endif