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
#include "sra_board.h"

static const char *TAG = "oled_example";

void app_main()
{
	// Declaring an empty u8g2_t struct to configure the oled
	u8g2_t oled_config;

	// Initialising the OLED
	init_oled(&oled_config);
	vTaskDelay(100);

	// Clearing the screen
	u8g2_ClearBuffer(&oled_config);

	// Setting the font and printing the string on screen
	u8g2_SetFont(&oled_config, u8g2_font_ncenB14_tr);
	u8g2_DrawStr(&oled_config, 2, 17, "Hello World!");

	// Sending the buffer to the screen
	u8g2_SendBuffer(&oled_config);

	ESP_LOGI(TAG, "Successfully displayed Hello World on the OLED Screen");
}