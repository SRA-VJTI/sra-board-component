/**
 * Code from https://github.com/yanbe/ssd1306-esp-idf-i2c.git is used as a starting point,
 * in addition to code from https://github.com/espressif/esp-iot-solution.
 *
 * Definitions are borrowed from:
 * http://robotcantalk.blogspot.com/2015/03/interfacing-arduino-with-ssd1306-driven.html
 *
 * For LVGL the forum has been used, in particular: https://blog.littlevgl.com/2019-05-06/oled
 */

/*********************
 *      INCLUDES
 *********************/
#include "lvgl_helpers.h"
#include "esp_log.h"
#include <string.h>

/*********************
 *      DEFINES
 *********************/

#define TAG "lvgl_helpers_ssd1306"

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static uint8_t send_data(lv_display_t *disp_drv, const void *bytes, size_t bytes_len);
static uint8_t send_pixels(lv_display_t *disp_drv, const uint8_t *color_buffer, size_t buffer_len);
static void convert_htiled_to_ssd1306(const uint8_t *src, uint32_t width, uint32_t height, uint8_t *dst);

/**********************
 *  STATIC VARIABLES
 **********************/

static i2c_dev_t ssd1306_dev_t;
static uint8_t ssd1306_framebuffer[DISP_BUF_SIZE];

/**********************
 *      MACROS
 **********************/

#define BIT_SET(a, b) ((a) |= (1U << (b)))
#define BIT_CLEAR(a, b) ((a) &= ~(1U << (b)))

void ssd1306_init(void)
{
    uint8_t orientation_1 = 0;
    uint8_t orientation_2 = 0;

    orientation_1 = OLED_CMD_SET_SEGMENT_REMAP;       // 0xA0;
    orientation_2 = OLED_CMD_SET_COM_SCAN_MODE_REMAP; // OLED_CMD_SET_COM_SCAN_MODE_NORMAL;

    uint8_t display_mode = 0;

    display_mode = OLED_CMD_DISPLAY_NORMAL;

    uint8_t conf[] = {
        OLED_CONTROL_BYTE_CMD_STREAM,
        OLED_CMD_SET_CHARGE_PUMP,
        0x14,
        orientation_1,
        orientation_2,
        OLED_CMD_SET_CONTRAST,
        display_mode,
        0xFF,
        OLED_CMD_DISPLAY_ON};

    uint8_t err = send_data(NULL, conf, sizeof(conf));
    assert(0 == err);
}

void ssd1306_flush(lv_display_t *disp_drv, const lv_area_t *area, uint8_t *px_map)
{
    /* Divide by 8 */
    uint8_t row1 = area->y1 >> 3;
    uint8_t row2 = area->y2 >> 3;
    uint32_t width = (uint32_t)(area->x2 - area->x1 + 1);
    uint32_t height = (uint32_t)(area->y2 - area->y1 + 1);
    LV_ASSERT((height % 8U) == 0U);

    uint8_t conf[] = {
        OLED_CONTROL_BYTE_CMD_STREAM,
        OLED_CMD_SET_MEMORY_ADDR_MODE,
        0x00,
        OLED_CMD_SET_COLUMN_RANGE,
        (uint8_t)area->x1,
        (uint8_t)area->x2,
        OLED_CMD_SET_PAGE_RANGE,
        row1,
        row2,
    };

    const uint8_t *htiled = px_map + LVGL_MONO_PALETTE_BYTES;
    convert_htiled_to_ssd1306(htiled, width, height, ssd1306_framebuffer);

    uint8_t err = send_data(disp_drv, conf, sizeof(conf));
    assert(0 == err);

    size_t page_cnt = (size_t)(row2 - row1 + 1);
    size_t payload_len = width * page_cnt;
    err = send_pixels(disp_drv, ssd1306_framebuffer, payload_len);
    assert(0 == err);

    lv_display_flush_ready(disp_drv);
}

void ssd1306_rounder_event_cb(lv_event_t *e)
{
    lv_display_t *disp = lv_event_get_target(e);
    lv_area_t *area = lv_event_get_param(e);
    if (!disp || !area)
    {
        return;
    }

    area->x1 = 0;
    area->y1 = 0;
    area->x2 = lv_display_get_horizontal_resolution(disp) - 1;
    area->y2 = lv_display_get_vertical_resolution(disp) - 1;
}

void ssd1306_sleep_in(void)
{
    uint8_t conf[] = {
        OLED_CONTROL_BYTE_CMD_STREAM,
        OLED_CMD_DISPLAY_OFF};

    uint8_t err = send_data(NULL, conf, sizeof(conf));
    assert(0 == err);
}

void ssd1306_sleep_out(void)
{
    uint8_t conf[] = {
        OLED_CONTROL_BYTE_CMD_STREAM,
        OLED_CMD_DISPLAY_ON};

    uint8_t err = send_data(NULL, conf, sizeof(conf));
    assert(0 == err);
}

/* Config the i2c master */
bool lvgl_i2c_driver_init(int sda_pin, int scl_pin, int speed_hz)
{
    CHECK(i2cdev_init());

    ssd1306_dev_t.port = I2C_NUM_1;
    ssd1306_dev_t.addr = OLED_I2C_ADDRESS;
    ssd1306_dev_t.cfg.mode = I2C_MODE_MASTER;
    ssd1306_dev_t.cfg.sda_io_num = sda_pin;
    ssd1306_dev_t.cfg.scl_io_num = scl_pin;
    ssd1306_dev_t.cfg.sda_pullup_en = GPIO_PULLUP_ENABLE;
    ssd1306_dev_t.cfg.scl_pullup_en = GPIO_PULLUP_ENABLE;
    ssd1306_dev_t.cfg.master.clk_speed = OLED_IIC_FREQ_HZ;

    return i2c_dev_create_mutex(&ssd1306_dev_t);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static uint8_t send_data(lv_display_t *disp_drv, const void *bytes, size_t bytes_len)
{
    (void)disp_drv;

    const uint8_t *data = (const uint8_t *)bytes;

    I2C_DEV_TAKE_MUTEX(&ssd1306_dev_t);
    I2C_DEV_CHECK(&ssd1306_dev_t, i2c_dev_write(&ssd1306_dev_t, NULL, 0, data, bytes_len));
    I2C_DEV_GIVE_MUTEX(&ssd1306_dev_t);

    return ESP_OK;
}

static uint8_t send_pixels(lv_display_t *disp_drv, const uint8_t *color_buffer, size_t buffer_len)
{
    (void)disp_drv;
    I2C_DEV_TAKE_MUTEX(&ssd1306_dev_t);
    uint8_t data = OLED_CONTROL_BYTE_DATA_STREAM;
    I2C_DEV_CHECK(&ssd1306_dev_t, i2c_dev_write(&ssd1306_dev_t, NULL, 0, &data, 1));

    uint8_t buffer[buffer_len + 1];
    buffer[0] = OLED_CONTROL_BYTE_DATA_STREAM;
    memcpy(&buffer[1], color_buffer, buffer_len);

    I2C_DEV_CHECK(&ssd1306_dev_t, i2c_dev_write(&ssd1306_dev_t, NULL, 0, &buffer, buffer_len + 1));
    I2C_DEV_GIVE_MUTEX(&ssd1306_dev_t);

    return ESP_OK;
}

static void convert_htiled_to_ssd1306(const uint8_t *src, uint32_t width, uint32_t height, uint8_t *dst)
{
    LV_ASSERT(src != NULL);
    LV_ASSERT(dst != NULL);
    LV_ASSERT((width * height / 8U) <= (uint32_t)sizeof(ssd1306_framebuffer));
    memset(dst, 0, width * height / 8U);

    for (uint32_t y = 0; y < height; y++)
    {
        for (uint32_t x = 0; x < width; x++)
        {
            uint32_t src_index = y * width + x;
            uint8_t pixel = (src[src_index / 8U] >> (7 - (src_index % 8U))) & 0x1U;
            uint32_t dest_index = x + ((y >> 3U) * width);
            uint8_t bit_index = y & 0x7U;
            if (pixel)
            {
                BIT_SET(dst[dest_index], bit_index);
            }
            else
            {
                BIT_CLEAR(dst[dest_index], bit_index);
            }
        }
    }
}
