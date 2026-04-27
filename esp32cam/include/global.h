#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "WiFi.h"
#include "esp_camera.h"

#define LED_PIN 33

extern QueueHandle_t aiResultQueue;

typedef struct struct_message {
    uint8_t direction;
} struct_message;

extern struct_message myData;

#endif