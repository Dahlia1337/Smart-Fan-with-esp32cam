// #include "global.h"
// #include "led_blinky.h"
// #include "neo_blinky.h"

// void setup()
// {
//     Serial.begin(115200);
//     delay(5000); // Đợi một chút để Serial có thể khởi động
//     Serial.println("Starting setup...");

// //     xTaskCreate(led_blinky, "Task LED Blink", 1024, NULL, 2, NULL);
// //     xTaskCreate(neo_blinky, "Task NEO Blink", 2048, NULL, 2, NULL);
// }

// void loop()
// {
//     Serial.println("Main loop is running...");
//     vTaskDelay(pdMS_TO_TICKS(1000));
// }



#include <Arduino.h>
#include "global.h"

// Bộ nhớ đệm tĩnh để chứa ảnh JPEG (Giới hạn 50KB là dư dả cho QVGA)
const int MAX_IMAGE_SIZE = 50 * 1024;
uint8_t image_buffer[MAX_IMAGE_SIZE];
uint32_t expected_size = 0;
uint32_t current_index = 0;
bool is_receiving = false;

void setup() {
    // Serial giao tiếp với Máy tính
    Serial.begin(115200); 
    delay(5000); // Đợi một chút để Serial có thể khởi động
    Serial.println("S3 đã sẵn sàng!");
    
    // Serial1 giao tiếp với ESP32-CAM
    Serial1.begin(1000000, SERIAL_8N1, RX_FROM_CAM, TX_TO_CAM);
    
    Serial.println("S3 đã sẵn sàng nhận ảnh từ CAM...");
}

void loop() {
    // 1. KIỂM TRA DỮ LIỆU TỪ CAM
    while (Serial1.available() > 0) {
        
        // Nếu chưa bắt đầu nhận ảnh, chờ đọc Header "IMG:xxx\n"
        if (!is_receiving) {
            String header = Serial1.readStringUntil('\n');
            if (header.startsWith("IMG:")) {
                expected_size = header.substring(4).toInt();
                
                if (expected_size > 0 && expected_size <= MAX_IMAGE_SIZE) {
                    current_index = 0;
                    is_receiving = true;
                    // Serial.printf("Bắt đầu nhận ảnh: %u bytes\n", expected_size);
                } else {
                    Serial.println("Lỗi: Kích thước ảnh không hợp lệ!");
                }
            }
        } 
        // Nếu đang trong quá trình nhận byte ảnh
        else {
            // Đọc hết mức có thể vào buffer
            int bytes_to_read = Serial1.available();
            
            // Chống tràn mảng
            if (current_index + bytes_to_read > expected_size) {
                bytes_to_read = expected_size - current_index;
            }
            
            int bytes_read = Serial1.readBytes(&image_buffer[current_index], bytes_to_read);
            current_index += bytes_read;

            // Nếu đã nhận đủ số byte
            if (current_index >= expected_size) {
                Serial.printf("--> Đã nhận xong 1 Frame! (%u bytes)\n", current_index);
                
                // Ở đây, image_buffer đang chứa chính xác file JPEG.
                // Các bước tiếp theo: Giải mã JPEG -> Đưa vào Tensor -> Chạy AI.
                
                is_receiving = false; // Reset trạng thái để đón ảnh mới
            }
        }
    }
    Serial.println("Can't receive data from CAM yet...");
    vTaskDelay(pdMS_TO_TICKS(1000)); // Delay để tránh spam Serial
}
