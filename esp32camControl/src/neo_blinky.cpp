// #include "neo_blinky.h"


// void neo_blinky(void *pvParameters)
// {

//     // Khởi tạo đối tượng NeoPixel
//     Adafruit_NeoPixel strip(LED_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);
//     strip.begin();
//     strip.clear();
//     strip.show();
//     strip.setBrightness(20);

//     while (1)
//     {

//         for (long firstPixelHue = 0; firstPixelHue < 65536; firstPixelHue += 256)
//         {
//             // Chuyển đổi giá trị Hue sang màu RGB
//             int pixelHue = firstPixelHue + (0 * 65536L / strip.numPixels());
//             strip.setPixelColor(0, strip.gamma32(strip.ColorHSV(pixelHue)));
//             strip.show();

//             vTaskDelay(pdMS_TO_TICKS(20));
//         }
//     }
// }