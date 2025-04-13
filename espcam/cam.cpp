#include "cam.h"
#include <Arduino.h>
#include <WebServer.h>

#define CAM_WIDTH 160
#define CAM_HEIGHT 120
#define MOTION_THRESHOLD 25    
#define CHANGE_LIMIT 1000       

static uint8_t previousFrame[CAM_WIDTH * CAM_HEIGHT];
static int lastMotionScore = 0;

bool initCamera() {
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = 5;
    config.pin_d1 = 18;
    config.pin_d2 = 19;
    config.pin_d3 = 21;
    config.pin_d4 = 36;
    config.pin_d5 = 39;
    config.pin_d6 = 34;
    config.pin_d7 = 35;
    config.pin_xclk = 0;
    config.pin_pclk = 22;
    config.pin_vsync = 25;
    config.pin_href = 23;
    config.pin_sscb_sda = 26;
    config.pin_sscb_scl = 27;
    config.pin_pwdn = 32;
    config.pin_reset = -1;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;

    if(psramFound()){
        config.frame_size = FRAMESIZE_QQVGA;
        config.jpeg_quality = 12;
        config.fb_count = 2;
        config.fb_location = CAMERA_FB_IN_PSRAM;
    } else {
        return false;
    }

    esp_err_t err = esp_camera_init(&config);
    return err == ESP_OK;
}

void captureAndServeImage(const String& command, WebServer& server) {
    if (command == "capture") {
        camera_fb_t* fb = esp_camera_fb_get();
        if (!fb) {
            Serial.println("Camera capture failed");
            server.send(500, "text/plain", "Camera capture failed");
            return;
        }

        // Send HTTP headers
        server.sendHeader("Content-Disposition", "inline; filename=capture.jpg");
        server.setContentLength(fb->len); // Set the content length
        server.send(200, "image/jpeg", ""); // Send the headers

        // Write the image data directly to the client
        server.client().write(fb->buf, fb->len);

        esp_camera_fb_return(fb);
        Serial.println("Image captured and served.");
    }
}