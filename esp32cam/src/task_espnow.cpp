#include "task_espnow.h"

// Ví dụ MAC là 9C:13:9E:BB:55:5C
uint8_t broadcastAddress[] = {0x9C, 0x13, 0x9E, 0xBB, 0x55, 0x5C};
esp_now_peer_info_t peerInfo;

void espnow_setup()
{
    if (esp_now_init() != ESP_OK)
    {
        return;
    }

    esp_now_peer_info_t peerInfo = {};

    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    peerInfo.ifidx = WIFI_IF_STA;

    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
    }
}

void task_espnow(void *pvParameters)
{
    uint8_t current_label = 3;

    while (1)
    {
        if (aiResultQueue != NULL)
        {
            if (xQueueReceive(aiResultQueue, &current_label, portMAX_DELAY) == pdPASS)
            {

                myData.direction = current_label;
                esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}