#include "task_camera.h"

bool cam_setup()
{
    camera_config_t config;
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
    // Ảnh màu
    config.pixel_format = PIXFORMAT_JPEG;

    // Thiết lập độ phân giải và chất lượng
    if (psramFound())
    {

        config.frame_size = FRAMESIZE_QVGA; // 320x240 - Đủ dùng cho TinyML và nhẹ
        config.jpeg_quality = 12;           // 0-63 (số càng nhỏ chất lượng càng cao)
        config.fb_count = 2;                // Dùng 2 buffer để tăng tốc độ
    }
    else
    {
        config.frame_size = FRAMESIZE_QVGA;
        config.jpeg_quality = 12;
        config.fb_count = 1;
    }

    // Khởi tạo Camera
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK)
    {
        // serial.printf("Camera start failed, error: 0x%x\n", err);
        return false;
    }
    return true;
};

void task_cam(void *pvParameters)
{
    while (1)
    {
        // 1. Chỉ làm việc khi máy tính (PC) gửi lệnh xuống
        if (Serial.available() > 0)
        {
            String req = Serial.readStringUntil('\n');
            req.trim();

            // 2. Nếu PC hô "REQ_FRAME" (Yêu cầu ảnh mới)
            if (req == "REQ_FRAME")
            {
                // Lúc này mới bắt đầu bấm máy chụp
                camera_fb_t *fb = esp_camera_fb_get();

                if (fb)
                {
                    // Đổi header từ "IMG:" thành "[PYTHON_IMG:]" cho khớp code Python
                    Serial.printf("[PYTHON_IMG:%u]\n", fb->len);

                    // Gửi cục data nhị phân
                    Serial.write(fb->buf, fb->len);     

                    // Trả lại bộ đệm
                    esp_camera_fb_return(fb);
                }
                else
                {
                    // Báo lỗi cho PC nếu chụp xịt
                    Serial.println("[PYTHON_IMG:0]");
                }
            }
            // 3. Nếu PC gửi kết quả AI trả về (VD: "AI:2")
            else if (req.startsWith("AI:"))
            {
                uint8_t ai_label = req.substring(3).toInt();

                if (aiResultQueue != NULL)
                {
                    BaseType_t status = xQueueSend(aiResultQueue, &ai_label, 0);

                    if (status != pdPASS)
                    {
                        // break
                    }
                }
            }
        }

        // Nghỉ 1ms để nhường CPU cho FreeRTOS (Không dùng delay 20ms nữa)
        vTaskDelay(pdMS_TO_TICKS(1));
    }
};
