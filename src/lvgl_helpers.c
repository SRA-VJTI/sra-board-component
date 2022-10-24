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

static uint8_t send_data(lv_disp_drv_t *disp_drv, void *bytes, size_t bytes_len);
static uint8_t send_pixels(lv_disp_drv_t *disp_drv, void *color_buffer, size_t buffer_len);

/**********************
 *  STATIC VARIABLES
 **********************/
static int i2c_port = I2C_NUM_0;
//static const unsigned int I2C_TIMEOUT_MS = 10; //1000
static i2c_dev_t ssd1306_dev_t;

/**********************
 *      MACROS
 **********************/

#define BIT_SET(a,b) ((a) |= (1U<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1U<<(b)))

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
bool i2c_initialised = false;

void ssd1306_init(void)
{
    uint8_t orientation_1 = 0;
    uint8_t orientation_2 = 0;

    orientation_1 = OLED_CMD_SET_SEGMENT_REMAP;// 0xA0;
    orientation_2 = OLED_CMD_SET_COM_SCAN_MODE_REMAP; //OLED_CMD_SET_COM_SCAN_MODE_NORMAL;

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
        OLED_CMD_DISPLAY_ON
    };

    uint8_t err = send_data(NULL, conf, sizeof(conf));
    assert(0 == err);
}

void ssd1306_set_px_cb(lv_disp_drv_t * disp_drv, uint8_t * buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y,
        lv_color_t color, lv_opa_t opa)
{
    uint16_t byte_index = x + (( y>>3 ) * buf_w);
    uint8_t  bit_index  = y & 0x7;

    if ((color.full == 0) && (LV_OPA_TRANSP != opa)) {
        BIT_SET(buf[byte_index], bit_index);
    } else {
        BIT_CLEAR(buf[byte_index], bit_index);
    }
}

void ssd1306_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    /* Divide by 8 */
    uint8_t row1 = area->y1 >> 3;
    uint8_t row2 = area->y2 >> 3;

    uint8_t conf[] = {
        OLED_CONTROL_BYTE_CMD_STREAM,
        OLED_CMD_SET_MEMORY_ADDR_MODE,
        0x00,
        OLED_CMD_SET_COLUMN_RANGE,
        (uint8_t) area->x1,
        (uint8_t) area->x2,
        OLED_CMD_SET_PAGE_RANGE,
        row1,
        row2,
    };

    uint8_t err = send_data(disp_drv, conf, sizeof(conf));
    assert(0 == err);
    err = send_pixels(disp_drv, color_p, OLED_COLUMNS * (1 + row2 - row1));
    assert(0 == err);

    lv_disp_flush_ready(disp_drv);
}

void ssd1306_rounder(lv_disp_drv_t * disp_drv, lv_area_t *area)
{
    uint8_t hor_max = disp_drv->hor_res;
    uint8_t ver_max = disp_drv->ver_res;

    area->x1 = 0;
    area->y1 = 0;
    area->x2 = hor_max - 1;
    area->y2 = ver_max - 1;
}

void ssd1306_sleep_in(void)
{
    uint8_t conf[] = {
        OLED_CONTROL_BYTE_CMD_STREAM,
        OLED_CMD_DISPLAY_OFF
    };

    uint8_t err = send_data(NULL, conf, sizeof(conf));
    assert(0 == err);
}

void ssd1306_sleep_out(void)
{
    uint8_t conf[] = {
        OLED_CONTROL_BYTE_CMD_STREAM,
        OLED_CMD_DISPLAY_ON
    };

    uint8_t err = send_data(NULL, conf, sizeof(conf));
    assert(0 == err);
}

/* Config the i2c master
 *
 * This should init the i2c master to be used on display and touch controllers.
 * So we should be able to know if the display and touch controllers shares the
 * same i2c master.
 */
bool lvgl_i2c_driver_init(int sda_pin, int scl_pin, int speed_hz)
{
    esp_err_t err;
    // i2c_initialised = false;

    // i2c_port = i2c_initialised ? I2C_NUM_1 : I2C_NUM_0;

    CHECK(i2cdev_init());

    ssd1306_dev_t.port = i2c_port;
    ssd1306_dev_t.addr = OLED_I2C_ADDRESS;
    ssd1306_dev_t.cfg.mode = I2C_MODE_MASTER;
    ssd1306_dev_t.cfg.sda_io_num = sda_pin;
    ssd1306_dev_t.cfg.scl_io_num = scl_pin;
    ssd1306_dev_t.cfg.sda_pullup_en = GPIO_PULLUP_ENABLE;
    ssd1306_dev_t.cfg.scl_pullup_en = GPIO_PULLUP_ENABLE;
    ssd1306_dev_t.cfg.master.clk_speed = OLED_IIC_FREQ_HZ;

    return i2c_dev_create_mutex(&ssd1306_dev_t);
    // ESP_LOGI(TAG, "Initializing I2C master port %d...", i2c_port);
    // ESP_LOGI(TAG, "SDA pin: %d, SCL pin: %d, Speed: %d (Hz)",
    //     sda_pin, scl_pin, speed_hz);
    
    // i2c_config_t conf = {
    //     .mode               = I2C_MODE_MASTER,
    //     .sda_io_num         = sda_pin,
    //     .sda_pullup_en      = GPIO_PULLUP_ENABLE,
    //     .scl_io_num         = scl_pin,
    //     .scl_pullup_en      = GPIO_PULLUP_ENABLE,
    //     .master.clk_speed   = speed_hz,
    // };

    // ESP_LOGI(TAG, "Setting I2C master configuration...");
    // err = i2c_param_config(i2c_port, &conf);
    // assert(ESP_OK == err);

    // ESP_LOGI(TAG, "Installing I2C master driver...");
    // err = i2c_driver_install(i2c_port,
    //     I2C_MODE_MASTER,
    //     0, 0 /*I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE */,
    //     0 /* intr_alloc_flags */);
    // assert(ESP_OK == err);

    return ESP_OK; // != err;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static uint8_t send_data(lv_disp_drv_t *disp_drv, void *bytes, size_t bytes_len)
{
    (void) disp_drv;
    esp_err_t err;

    uint8_t *data = (uint8_t *) bytes;

    I2C_DEV_TAKE_MUTEX(&ssd1306_dev_t);
    I2C_DEV_CHECK(&ssd1306_dev_t, i2c_dev_write(&ssd1306_dev_t, NULL, 0, data, bytes_len));
    I2C_DEV_GIVE_MUTEX(&ssd1306_dev_t);

    // i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    // i2c_master_start(cmd);
    // i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);

    // for (size_t idx = 0; idx < bytes_len; idx++) {
    //     i2c_master_write_byte(cmd, data[idx], true);
    // }

    // i2c_master_stop(cmd);

    // /* Send queued commands */
    // err = i2c_master_cmd_begin(i2c_port, cmd, I2C_TIMEOUT_MS / portTICK_PERIOD_MS);
    // i2c_cmd_link_delete(cmd);

    return ESP_OK; // == err ? 0 : 1;
}

static uint8_t send_pixels(lv_disp_drv_t *disp_drv, void *color_buffer, size_t buffer_len)
{
    (void) disp_drv;
    esp_err_t err;

    //CHECK(lvgl_i2c_driver_init(OLED_SDA, OLED_SCL, OLED_IIC_FREQ_HZ));
    I2C_DEV_TAKE_MUTEX(&ssd1306_dev_t);
    uint8_t data = 0x40;
    I2C_DEV_CHECK(&ssd1306_dev_t, i2c_dev_write(&ssd1306_dev_t, NULL, 0, &data, 1));
    uint8_t *color_buffers = (uint8_t *)color_buffer;
    I2C_DEV_CHECK(&ssd1306_dev_t, i2c_dev_write_oled(&ssd1306_dev_t, NULL, 0, color_buffers, buffer_len));
    I2C_DEV_GIVE_MUTEX(&ssd1306_dev_t);

    // i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    // i2c_master_start(cmd);
    // i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);

    // i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_DATA_STREAM, true);
    // i2c_master_write(cmd, (uint8_t *) color_buffer, buffer_len, true);
    // i2c_master_stop(cmd);

    // /* Send queued commands */
    // err = i2c_master_cmd_begin(i2c_port, cmd, 10 / portTICK_PERIOD_MS);
    // i2c_cmd_link_delete(cmd);

    return ESP_OK ; //== err ? 0 : 1;
}
