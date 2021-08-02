/* Copyright 2021 Google LLC All Rights Reserved.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
  ==============================================================================
  */
#include "esp_camera.h"

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22
#define LB 33


const int kNumCols = 96;
const int kNumRows = 96;
const int kNumChannels = 1;
const int bytesPerFrame = kNumCols * kNumRows;
uint8_t data[kNumCols * kNumRows * kNumChannels];
camera_fb_t * fb = NULL;
camera_config_t config;
 
// Get an image from the camera module
boolean GetImage(int image_width,
                 int image_height, int channels, uint8_t* image_data) {
  static bool g_is_camera_initialized = false;

  if (!g_is_camera_initialized) {
    esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return false;
  }
    g_is_camera_initialized = true;
  }

fb = esp_camera_fb_get();

    // Trimming Image
    int post = 0;
    int startx = (fb->width - kNumCols) / 2;
    int starty = (fb->height - kNumRows) / 2;
    for (int y = 0; y < kNumRows; y++) {
      for (int x = 0; x < kNumCols; x++) {
        int getPos = (starty + y) * fb->width + startx + x;
        image_data[post] = fb->buf[getPos];
        post++;
      }
    }
  
esp_camera_fb_return(fb);
  return true;
}


void setup() {
  pinMode(LB, OUTPUT);
      digitalWrite(LB, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(400);                       // wait for a second
    digitalWrite(LB, LOW);    // turn the LED off by making the voltage LOW
    delay(400);                       // wait for a second
      digitalWrite(LB, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(400);                       // wait for a second
    digitalWrite(LB, LOW);    // turn the LED off by making the voltage LOW
    delay(400);                       // wait for a second
  
  Serial.begin(9600);
  while (!Serial);

  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format =  PIXFORMAT_GRAYSCALE;
  config.frame_size = FRAMESIZE_QQVGA;
  config.jpeg_quality = 10;
  config.fb_count = 1;
    
}



void flushCap() {
  for (int i = 0; i < kNumCols * kNumRows * kNumChannels; i++) {
    data[i] = 0;
  }
}

void loop() {
//  Serial.println(000"creating image");
  GetImage(kNumCols, kNumRows, kNumChannels, data);
//  Serial.println("got image");
  Serial.write(data, bytesPerFrame);
//  flushCap();
}
