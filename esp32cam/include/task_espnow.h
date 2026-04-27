#ifndef _TASK_ESPNOW_H_
#define _TASK_ESPNOW_H_

#include "global.h"
#include <Arduino.h>
#include "esp_now.h"

void espnow_setup();
void task_espnow(void *pvParameters);

#endif // _TASK_ESPNOW_H_