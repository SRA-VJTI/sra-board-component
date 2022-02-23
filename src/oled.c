/*
 * MIT License
 *
 * Copyright (c)  2021 Society of Robotics and Automation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "oled.h"

static const char *TAG_OLED = "oled";

const unsigned char sra_logo[] = {
0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
0b11111111,0b10000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00111111,0b11111111,
0b11111110,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00001111,0b11111111,
0b11111110,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00001111,0b11111111,
0b11111100,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000111,0b11111111,
0b11111100,0b00000000,0b00000000,0b00000001,0b00000000,0b00000000,0b00000000,0b00000111,0b11111111,
0b11111100,0b00000000,0b00000000,0b00000000,0b10000000,0b00000000,0b00000000,0b00000111,0b11111111,
0b11111100,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000111,0b11111111,
0b11111100,0b00000000,0b00000000,0b00000000,0b01000000,0b00000000,0b00000000,0b00000111,0b11111111,
0b11111100,0b01111100,0b11111000,0b00111111,0b11111110,0b00000000,0b00111100,0b00000111,0b11111111,
0b11111100,0b11111100,0b11111100,0b00111111,0b11111111,0b00000000,0b01111100,0b00000111,0b11111111,
0b11111101,0b11111100,0b11111110,0b00111111,0b11111111,0b00000000,0b01111110,0b00000111,0b11111111,
0b11111101,0b11100000,0b00011100,0b00000000,0b00110111,0b00000000,0b11111110,0b00000111,0b11111111,
0b11111101,0b11100000,0b00000000,0b00000000,0b00110111,0b00000001,0b11111111,0b00000111,0b11111111,
0b11111101,0b11111100,0b11111000,0b00000000,0b00111111,0b00000001,0b11100111,0b10000111,0b11111111,
0b11111100,0b11111100,0b11111100,0b00111111,0b11111111,0b00000011,0b11000111,0b10000111,0b11111111,
0b11111100,0b01111000,0b11111110,0b00111111,0b11111111,0b00000011,0b11000011,0b11000111,0b11111111,
0b11111100,0b00000000,0b00011110,0b00111111,0b01111111,0b00000111,0b10000011,0b11000111,0b11111111,
0b11111101,0b11100000,0b00011110,0b00111000,0b00110111,0b00000111,0b00000001,0b11100111,0b11111111,
0b11111111,0b11111100,0b11111100,0b00111000,0b00110111,0b00001111,0b00000001,0b11100111,0b11111111,
0b11111110,0b11111100,0b11111100,0b00111000,0b01100111,0b00011110,0b00000000,0b11110111,0b11111111,
0b11111111,0b00111000,0b01110000,0b00110000,0b01100111,0b00000000,0b00000000,0b00000111,0b11111111,
0b11111111,0b00000000,0b00000000,0b00000000,0b11100000,0b00000000,0b00000000,0b00000111,0b11111111,
0b11111111,0b10000000,0b00000000,0b00000001,0b11000000,0b00000000,0b00000000,0b00000111,0b11111111,
0b11111111,0b11000000,0b00000000,0b00000011,0b10000000,0b00000000,0b00000000,0b00000111,0b11111111,
0b11111111,0b11100000,0b00000000,0b00000111,0b10000000,0b00000000,0b00000000,0b00000111,0b11111111,
0b11111111,0b11111000,0b00000000,0b00011111,0b00000000,0b00000000,0b00000000,0b00000111,0b11111111,
0b11111111,0b11111110,0b00000000,0b01111110,0b00000000,0b00000000,0b00000000,0b00000111,0b11111111,
0b11111111,0b11111111,0b11111111,0b11111000,0b00000000,0b00000000,0b00000000,0b00000111,0b11111111,
0b11111111,0b11111111,0b11111111,0b11110000,0b00000000,0b00000000,0b00000000,0b00001111,0b11111111,
0b11111111,0b11111111,0b11111111,0b11000000,0b00000000,0b00000000,0b00000000,0b00111111,0b11111111,
0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,
};

esp_err_t init_oled(u8g2_t* u8g2)
{

    u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
	u8g2_esp32_hal.sda  = OLED_SDA;
	u8g2_esp32_hal.scl  = OLED_SCL;
	u8g2_esp32_hal_init(u8g2_esp32_hal);

	u8g2_Setup_ssd1306_i2c_128x32_univision_f(
		u8g2,
		U8G2_R0,
		u8g2_esp32_i2c_byte_cb,
		u8g2_esp32_gpio_and_delay_cb);

	u8x8_SetI2CAddress(&u8g2->u8x8,0x78);

	u8g2_InitDisplay(u8g2); 

	u8g2_SetPowerSave(u8g2, 0); 
    u8g2_ClearBuffer(u8g2);
    display_logo(u8g2);
    u8g2_SendBuffer(u8g2);

	ESP_LOGI(TAG_OLED, "Configured OLED Display");

    return ESP_OK;
}

esp_err_t display_logo(u8g2_t* u8g2)
{
    u8g2_DrawBox(u8g2, 0, 0, 128, 64);
    u8g2_DrawBitmap(u8g2,27,0,9,36,sra_logo);

	ESP_LOGI(TAG_OLED, "Displayed SRA logo on OLED Screen");

    return ESP_OK;
}
