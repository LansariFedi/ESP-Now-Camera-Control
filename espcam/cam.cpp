#include "cam.h"
#include <Arduino.h>
#include <WebServer.h>

#define CAM_WIDTH 160
#define CAM_HEIGHT 120

static uint8_t* lastCapturedImage = nullptr; 
static size_t lastCapturedImageSize = 0;

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

    if (psramFound()) {
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

void captureImage() {
    if (lastCapturedImage) {
        free(lastCapturedImage);
        lastCapturedImage = nullptr;
        lastCapturedImageSize = 0;
    }

    // Capture a new image
    camera_fb_t* fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Camera capture failed");
        return;
    }

    // Allocate memory for the new image
    lastCapturedImage = (uint8_t*)malloc(fb->len);
    if (lastCapturedImage) {
        memcpy(lastCapturedImage, fb->buf, fb->len);
        lastCapturedImageSize = fb->len;
        Serial.println("Image captured and stored.");
    } else {
        Serial.println("Failed to allocate memory for the captured image.");
    }

    esp_camera_fb_return(fb);
}

void serveLastCapturedImage(WebServer& server) {
    if (lastCapturedImage && lastCapturedImageSize > 0) {
        // Send HTTP headers
        server.sendHeader("Content-Disposition", "inline; filename=capture.jpg");
        server.setContentLength(lastCapturedImageSize);  
        server.send(200, "image/jpeg", "");

        server.client().write(lastCapturedImage, lastCapturedImageSize);
        //Serial.println("Last captured image served.");
    } else {
        server.send(404, "text/plain", "No image available. Please capture an image first.");
        //Serial.println("No image available to serve.");
    }
}