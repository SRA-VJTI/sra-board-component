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
    // Initialising the OLED
    init_oled();
    vTaskDelay(100);

    // Clearing the screen
    lv_obj_clean(lv_scr_act());

    // Printing the string on screen    
    lv_obj_t *text_msg = lv_label_create(lv_scr_act());
    lv_label_set_text(text_msg, "Hello from SRA");
    lv_obj_align(text_msg, LV_ALIGN_CENTER, 0, 0);

    // Refresh Display
    lv_refr_now(NULL);

	ESP_LOGI(TAG, "Successfully displayed Hello World on the OLED Screen");
}

