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

unsigned char sra_logo[] = {
	0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b01111111, 0b11111111,
	0b11111111, 0b11100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00011111, 0b11111111,
	0b11111111, 0b11000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001111, 0b11111111,
	0b11111111, 0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000111, 0b11111111,
	0b11111111, 0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000110, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000111, 0b11111111,
	0b11111111, 0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000011, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000111, 0b11111111,
	0b11111111, 0b10000111, 0b11111110, 0b00011111, 0b11110000, 0b00111111, 0b11111111, 0b11111111, 0b10000000, 0b00000000, 0b01111111, 0b10000000, 0b00000111, 0b11111111,
	0b11111111, 0b10001111, 0b11111100, 0b00011111, 0b11111000, 0b00111111, 0b11111111, 0b11111111, 0b11000000, 0b00000000, 0b11111111, 0b11000000, 0b00000111, 0b11111111,
	0b11111111, 0b10001111, 0b11000000, 0b00000000, 0b10010000, 0b00000000, 0b00000000, 0b11101111, 0b11000000, 0b00000001, 0b11111111, 0b11100000, 0b00000111, 0b11111111,
	0b11111111, 0b10001111, 0b11111100, 0b00011111, 0b10000000, 0b00000000, 0b00000000, 0b01101111, 0b11000000, 0b00000111, 0b11110011, 0b11110000, 0b00000111, 0b11111111,
	0b11111111, 0b10000111, 0b11111110, 0b00011111, 0b11110000, 0b00111111, 0b11111111, 0b11111111, 0b10000000, 0b00001111, 0b11100001, 0b11111100, 0b00000111, 0b11111111,
	0b11111111, 0b11000000, 0b00000000, 0b00000011, 0b11111000, 0b00111111, 0b11111111, 0b11111111, 0b10000000, 0b00011111, 0b10000000, 0b11111110, 0b00000111, 0b11111111,
	0b11111111, 0b11000111, 0b10000000, 0b00000001, 0b11111000, 0b00111111, 0b00000000, 0b11101111, 0b11000000, 0b00111111, 0b00000000, 0b00111111, 0b00000111, 0b11111111,
	0b11111111, 0b11101111, 0b11111100, 0b00011111, 0b11111000, 0b00111111, 0b00000001, 0b11100111, 0b11000000, 0b11111110, 0b00000000, 0b00011111, 0b10000111, 0b11111111,
	0b11111111, 0b11110111, 0b11111110, 0b00011111, 0b11110000, 0b00111111, 0b00000011, 0b11000111, 0b11000001, 0b11111100, 0b00000000, 0b00001111, 0b11100111, 0b11111111,
	0b11111111, 0b11111000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000111, 0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000111, 0b11111111,
	0b11111111, 0b11111110, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00011111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000111, 0b11111111,
	0b11111111, 0b11111111, 0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b01111110, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000111, 0b11111111,
	0b11111111, 0b11111111, 0b11110000, 0b00000000, 0b00000000, 0b00000011, 0b11111000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00111111, 0b11100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00011111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b01111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111};

const unsigned char walle_logo[] = {
	0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
	0b00000000, 0b00000000, 0b00011111, 0b11111110, 0b01100111, 0b00111111, 0b11111100, 0b00000000, 0b00000000,
	0b00000000, 0b00000000, 0b01110011, 0b00000110, 0b01110110, 0b01110000, 0b01100110, 0b00000000, 0b00000000,
	0b00000000, 0b00000000, 0b01100011, 0b00001111, 0b11110111, 0b11110000, 0b11100110, 0b00000000, 0b00000000,
	0b00000000, 0b00000000, 0b00111001, 0b11111111, 0b01110110, 0b11111111, 0b10011100, 0b00000000, 0b00000000,
	0b00000000, 0b00000000, 0b00000010, 0b00000000, 0b11110111, 0b10000000, 0b01000000, 0b00000000, 0b00000000,
	0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b01111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
	0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11000011, 0b10000000, 0b00000000, 0b00000000, 0b00000000,
	0b00000000, 0b00000000, 0b00001101, 0b11111111, 0b11111111, 0b11111111, 0b11111000, 0b00000000, 0b00000000,
	0b00000000, 0b00000000, 0b00011001, 0b11110011, 0b00000000, 0b01000111, 0b10001100, 0b00000000, 0b00000000,
	0b00000000, 0b00000000, 0b00111110, 0b00110011, 0b00000000, 0b01000110, 0b00111110, 0b00000000, 0b00000000,
	0b00000000, 0b00000000, 0b01100000, 0b11100011, 0b00000000, 0b11000111, 0b10000111, 0b00000000, 0b00000000,
	0b00000000, 0b00000000, 0b00111111, 0b11000011, 0b11111111, 0b11000011, 0b11111110, 0b00000000, 0b00000000,
	0b00000000, 0b00000000, 0b00000001, 0b10000000, 0b00000000, 0b00000001, 0b11000000, 0b00000000, 0b00000000,
	0b00000000, 0b00011111, 0b11111111, 0b10000000, 0b00000000, 0b00000001, 0b11111111, 0b11111100, 0b00000000,
	0b00000000, 0b00111000, 0b01111001, 0b10000000, 0b00000000, 0b00000001, 0b10011110, 0b00001100, 0b00000000,
	0b00000000, 0b00111000, 0b00000001, 0b11111111, 0b11111111, 0b11111111, 0b10000000, 0b00001100, 0b00000000,
	0b00000000, 0b00111000, 0b11111001, 0b10000011, 0b00000000, 0b11100001, 0b10001111, 0b10001100, 0b00000000,
	0b00000000, 0b00111000, 0b11111001, 0b10000011, 0b00000000, 0b01000001, 0b10011111, 0b10001100, 0b00000000,
	0b00000000, 0b00111000, 0b00000001, 0b11111110, 0b00000000, 0b01111111, 0b10000000, 0b00001100, 0b00000000,
	0b00000000, 0b00111000, 0b00000001, 0b10000000, 0b00000000, 0b00000001, 0b10000000, 0b00001100, 0b00000000,
	0b00000000, 0b00011111, 0b11111111, 0b10000000, 0b00000000, 0b00000000, 0b11111111, 0b11111100, 0b00000000,
	0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001111, 0b11000000,
	0b00011000, 0b11000110, 0b00011111, 0b11000001, 0b10000000, 0b00110000, 0b00000000, 0b00111111, 0b11111000,
	0b00011001, 0b11001110, 0b00111000, 0b01110001, 0b10000000, 0b00111000, 0b00000100, 0b01000111, 0b11111100,
	0b00011101, 0b11101110, 0b00111111, 0b11110001, 0b11000000, 0b00111000, 0b00000000, 0b01100001, 0b11111100,
	0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00111111, 0b11111000,
	0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001111, 0b11000000};

const unsigned char mario_logo[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf0, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
	0xff, 0x87, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x0d, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x86, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xf8, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x3f, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xfe, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xf0,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x07, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x7f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xf8, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff,
	0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x03,
	0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00,
	0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0,
	0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xc0,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

esp_err_t init_oled_walle(u8g2_t *u8g2)
{
	u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
	u8g2_esp32_hal.sda = OLED_SDA;
	u8g2_esp32_hal.scl = OLED_SCL;
	u8g2_esp32_hal_init(u8g2_esp32_hal);

	u8g2_Setup_ssd1306_i2c_128x32_univision_f(
		u8g2,
		U8G2_R0,
		u8g2_esp32_i2c_byte_cb,
		u8g2_esp32_gpio_and_delay_cb);

	u8x8_SetI2CAddress(&u8g2->u8x8, 0x78);

	u8g2_InitDisplay(u8g2);

	u8g2_SetPowerSave(u8g2, 0);

	// Display SRA Logo
	display_sra_logo(u8g2);
	vTaskDelay(100);

	// Display Wall-E logo
	display_walle_logo(u8g2);
	vTaskDelay(100);

	// Clear the screen
	u8g2_ClearBuffer(u8g2);
	u8g2_SendBuffer(u8g2);
	ESP_LOGI(TAG_OLED, "Configured OLED Display");

	return ESP_OK;
}

esp_err_t init_oled_mario(u8g2_t *u8g2)
{
	u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
	u8g2_esp32_hal.sda = OLED_SDA;
	u8g2_esp32_hal.scl = OLED_SCL;
	u8g2_esp32_hal_init(u8g2_esp32_hal);

	u8g2_Setup_ssd1306_i2c_128x32_univision_f(
		u8g2,
		U8G2_R0,
		u8g2_esp32_i2c_byte_cb,
		u8g2_esp32_gpio_and_delay_cb);

	u8x8_SetI2CAddress(&u8g2->u8x8, 0x78);

	u8g2_InitDisplay(u8g2);

	u8g2_SetPowerSave(u8g2, 0);

	// Display SRA Logo
	display_sra_logo(u8g2);
	vTaskDelay(100);

	// Display Wall-E logo
	display_mario_logo(u8g2);
	vTaskDelay(100);

	// Clear the screen
	u8g2_ClearBuffer(u8g2);
	u8g2_SendBuffer(u8g2);
	ESP_LOGI(TAG_OLED, "Configured OLED Display");

	return ESP_OK;
}

esp_err_t display_sra_logo(u8g2_t *u8g2)
{
	// Clear the screen
	u8g2_ClearBuffer(u8g2);

	// Fill the screen
	u8g2_DrawBox(u8g2, 0, 0, 128, 64);

	// Draw SRA logo on the screen
	u8g2_DrawBitmap(u8g2, 8, 2, 14, 29, sra_logo);
	ESP_LOGI(TAG_OLED, "Displayed SRA logo on OLED Screen");

	// Sends the buffer to the OLED Display
	u8g2_SendBuffer(u8g2);

	return ESP_OK;
}

esp_err_t display_walle_logo(u8g2_t *u8g2)
{
	// Clear the screen
	u8g2_ClearBuffer(u8g2);

	// Draw Wall-E logo on the screen
	u8g2_DrawBitmap(u8g2, 27, 0, 9, 22, walle_logo);
	u8g2_SetFont(u8g2, u8g2_font_lucasfont_alternate_tf);

	// print "Wall-e" text on the screen
	u8g2_DrawStr(u8g2, 17, 30, "W a l l - E  2 0 2 2");

	// Sends the buffer to the OLED Display
	u8g2_SendBuffer(u8g2);
	ESP_LOGI(TAG_OLED, "Displayed Wall-E logo on OLED Screen");

	return ESP_OK;
}

esp_err_t display_mario_logo(u8g2_t *u8g2)
{
	// Clear the screen
	u8g2_ClearBuffer(u8g2);

	// Draw MARIO logo on the screen
	u8g2_DrawBitmap(u8g2, 27, 0, 9, 22, mario_logo);
	u8g2_SetFont(u8g2, u8g2_font_lucasfont_alternate_tf);

	// print "MARIO 2022" text on the screen
	u8g2_DrawStr(u8g2, 17, 30, "M A R I O  2 0 2 2");

	// Sends the buffer to the OLED Display
	u8g2_SendBuffer(u8g2);
	ESP_LOGI(TAG_OLED, "Displayed MARIO logo on OLED Screen");

	return ESP_OK;
}

esp_err_t display_lsa(line_sensor_array readings, u8g2_t *u8g2)
{
	// clear the screen
	u8g2_ClearBuffer(u8g2);

	// plot the bar of LSA 0
	u8g2_DrawFrame(u8g2, 27, 0, 10, 30);
	u8g2_DrawBox(u8g2, 27, 0, 10, readings.adc_reading[3] * 0.03);

	// plot the bar of LSA 1
	u8g2_DrawFrame(u8g2, 47, 0, 10, 30);
	u8g2_DrawBox(u8g2, 47, 0, 10, readings.adc_reading[2] * 0.03);

	// plot the bar of LSA 2
	u8g2_DrawFrame(u8g2, 67, 0, 10, 30);
	u8g2_DrawBox(u8g2, 67, 0, 10, readings.adc_reading[1] * 0.03);

	// plot the bar of LSA 3
	u8g2_DrawFrame(u8g2, 87, 0, 10, 30);
	u8g2_DrawBox(u8g2, 87, 0, 10, readings.adc_reading[0] * 0.03);

	// Sends the buffer to the OLED Display
	u8g2_SendBuffer(u8g2);

	return ESP_OK;
}

esp_err_t display_mpu(float pitch, float roll, u8g2_t *u8g2)
{

	// Clearing the screen
	u8g2_ClearBuffer(u8g2);

	// Setting the font and printing the string on screen
	u8g2_SetFont(u8g2, u8g2_font_ncenB10_tr);

	// Printing pitch on oled
	char pitch_str[20];
	sprintf(pitch_str, "Pitch : %0.2f", pitch);
	u8g2_DrawStr(u8g2, 2, 15, pitch_str);

	// Printing roll on oled
	char roll_str[20];
	sprintf(roll_str, "Roll : %0.2f", roll);
	u8g2_DrawStr(u8g2, 2, 30, roll_str);

	// Sending the buffer to the screen
	u8g2_SendBuffer(u8g2);

	return ESP_OK;
}

esp_err_t display_pid_values(float kp, float ki, float kd, u8g2_t *u8g2)
{
	// Clearing the screen
	u8g2_ClearBuffer(u8g2);

	// Setting the font and printing the string on screen
	u8g2_SetFont(u8g2, u8g_font_profont11);

	char kp_str[10], ki_str[10], kd_str[10];

	// Printing kp value on oled
	sprintf(kp_str, "Kp: %0.2f", kp);
	u8g2_DrawStr(u8g2, 0, 10, kp_str);

	// Printing ki value on oled
	sprintf(ki_str, "Ki: %0.2f", ki);
	u8g2_DrawStr(u8g2, 30, 20, ki_str);

	// Printing kd value on oled
	sprintf(kd_str, "Kd: %0.2f", kd);
	u8g2_DrawStr(u8g2, 60, 30, kd_str);

	// Sending the buffer to the screen
	u8g2_SendBuffer(u8g2);

	return ESP_OK;
}

esp_err_t display_servo_values(int s1, int s2, int s3, int s4, u8g2_t *u8g2)
{
	// Clearing the screen
	u8g2_ClearBuffer(u8g2);

	// Setting the font and printing the string on screen
	u8g2_SetFont(u8g2, u8g_font_profont11);

	char sA_str[20], sB_str[20], sC_str[20], sD_str[20];

	// Printing Base angle value on oled
	sprintf(sA_str, "Base     : %d", s1);
	u8g2_DrawStr(u8g2, 0, 8, sA_str);

	// Printing Shoulder angle value on oled
	sprintf(sB_str, "Shoulder : %d", s2);
	u8g2_DrawStr(u8g2, 0, 16, sB_str);

	// Printing Elbow angle C value on oled
	sprintf(sC_str, "Elbow    : %d", s3);
	u8g2_DrawStr(u8g2, 0, 24, sC_str);

	// Printing Claw angle value on oled
	sprintf(sD_str, "Claw     : %d", s4);
	u8g2_DrawStr(u8g2, 0, 32, sD_str);

	// Sending the buffer to the screen
	u8g2_SendBuffer(u8g2);

	return ESP_OK;
}