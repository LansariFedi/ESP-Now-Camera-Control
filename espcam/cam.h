#ifndef CAM_H
#define CAM_H

#include "esp_camera.h"
#include <Arduino.h>
#include <WebServer.h>

bool initCamera();
void captureAndServeImage(const String& command, WebServer& server); // Updated declaration

#endif