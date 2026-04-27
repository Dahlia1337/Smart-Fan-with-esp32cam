#include "global.h"
#include "task_camera.h"
#include "led_blinky.h"
#include "task_espnow.h"

void setup()
{
    Serial.begin(921600);
    delay(5000);
    Serial.println("\n--- ESP32-CAM START---");

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    aiResultQueue = xQueueCreate(5, sizeof(uint8_t));

    led_setup();
    espnow_setup();
    

    Serial.setDebugOutput(true);
    if (!cam_setup())
    {
        //serial.println("SYSTEM STOP.");
        while (true)
        {
            delay(1000);
        }
    }

    //serial.println("CAMERA START");
    xTaskCreate(led_blinky, "LED Blinky", 1024, NULL, 1, NULL);
    xTaskCreate(task_cam, "Task Camera", 8192, NULL, 2, NULL);
    xTaskCreate(task_espnow, "Task ESP-NOW", 8192, NULL, 3, NULL);
}

void loop()
{
    // Không làm gì ở loop chính, để FreeRTOS tự quản lý các Task
    // Serial.println("---ESP32cam are working---");
    vTaskDelay(5000);
}
