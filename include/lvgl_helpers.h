/**
* * MIT License
* Copyright (c) 2020 LVGL
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:

* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.

* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
**/
#ifndef LVGL_HELPERS_H
#define LVGL_HELPERS_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdbool.h>
#include "i2cdev.h"
#include "driver/i2c.h"
#include "assert.h"
#include "lvgl.h"
#include "pin_defs.h"

/*********************
 *      DEFINES
 *********************/

// SLA (0x3C) + WRITE_MODE (0x00) =  0x78 (0b01111000)
#define OLED_I2C_ADDRESS                    0x3C
#define OLED_WIDTH                          128
#define OLED_HEIGHT                         64
#define OLED_COLUMNS                        128
#define OLED_PAGES                          8
#define OLED_PIXEL_PER_PAGE                 8

// Control byte
#define OLED_CONTROL_BYTE_CMD_SINGLE        0x80
#define OLED_CONTROL_BYTE_CMD_STREAM        0x00
#define OLED_CONTROL_BYTE_DATA_STREAM       0x40

// Fundamental commands (pg.28)
#define OLED_CMD_SET_CONTRAST               0x81    // follow with 0x7F
#define OLED_CMD_DISPLAY_RAM                0xA4
#define OLED_CMD_DISPLAY_ALLON              0xA5
#define OLED_CMD_DISPLAY_NORMAL             0xA6
#define OLED_CMD_DISPLAY_INVERTED           0xA7
#define OLED_CMD_DISPLAY_OFF                0xAE
#define OLED_CMD_DISPLAY_ON                 0xAF

// Addressing Command Table (pg.30)
#define OLED_CMD_SET_MEMORY_ADDR_MODE       0x20    // follow with 0x00 = HORZ mode
#define OLED_CMD_SET_COLUMN_RANGE           0x21    // can be used only in HORZ/VERT mode - follow with 0x00 and 0x7F = COL127
#define OLED_CMD_SET_PAGE_RANGE             0x22    // can be used only in HORZ/VERT mode - follow with 0x00 and 0x07 = PAGE7

// Hardware Config (pg.31)
#define OLED_CMD_SET_DISPLAY_START_LINE     0x40
#define OLED_CMD_SET_SEGMENT_REMAP          0xA1
#define OLED_CMD_SET_MUX_RATIO              0xA8    // follow with 0x3F = 64 MUX
#define OLED_CMD_SET_COM_SCAN_MODE_NORMAL   0xC0
#define OLED_CMD_SET_COM_SCAN_MODE_REMAP    0xC8
#define OLED_CMD_SET_DISPLAY_OFFSET         0xD3    // follow with 0x00
#define OLED_CMD_SET_COM_PIN_MAP            0xDA    // follow with 0x12
#define OLED_CMD_NOP                        0xE3    // NOP

// Timing and Driving Scheme (pg.32)
#define OLED_CMD_SET_DISPLAY_CLK_DIV        0xD5    // follow with 0x80
#define OLED_CMD_SET_PRECHARGE              0xD9    // follow with 0xF1
#define OLED_CMD_SET_VCOMH_DESELCT          0xDB    // follow with 0x30

// Charge Pump (pg.62)
#define OLED_CMD_SET_CHARGE_PUMP            0x8D    // follow with 0x14

#define OLED_IIC_FREQ_HZ                    400000  // I2C clock frequency

/* DISP_BUF_SIZE value doesn't have an special meaning, but it's the size
 * of the buffer(s) passed to LVGL as display buffers. The default values used
 * were the values working for the contributor of the display controller.
 *
 * As LVGL supports partial display updates the DISP_BUF_SIZE doesn't
 * necessarily need to be equal to the display size.
 *
 * When using RGB displays the display buffer size will also depends on the
 * color format being used, for RGB565 each pixel needs 2 bytes.
 * When using the mono theme, the display pixels can be represented in one bit,
 * so the buffer size can be divided by 8, e.g. see SSD1306 display size. */

#define DISP_BUF_SIZE   1024

// #define DISP_BUF_SIZE  (LV_HOR_RES_MAX * LV_VER_RES_MAX)
// #if defined (CONFIG_CUSTOM_DISPLAY_BUFFER_SIZE)
// #endif
// #if defined (CONFIG_LV_THEME_MONO)
//     #define DISP_BUF_SIZE  (LV_HOR_RES_MAX * (LV_VER_RES_MAX / 8))
// #else
// #endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/* Initialize I2C master  */
bool lvgl_i2c_driver_init(int sda_pin, int scl_pin, int speed);
/* Initialize ssd1306 device  */
void ssd1306_init(void);

void ssd1306_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_map);
void ssd1306_rounder(lv_disp_drv_t * disp_drv, lv_area_t *area);
void ssd1306_set_px_cb(lv_disp_drv_t * disp_drv, uint8_t * buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y, lv_color_t color, lv_opa_t opa);
void ssd1306_sleep_in(void);
void ssd1306_sleep_out(void);

/**********************
 *      MACROS
 **********************/
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LVGL_HELPERS_H */


