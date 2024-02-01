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

#include "adc.h"

static const char *TAG = "adc";
static const int adc_io[5] = {LSA_A0, LSA_A1, LSA_A2, LSA_A3, LSA_A4};

esp_err_t config_adc1(adc_obj_t *adc_obj)
{
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_LEN,
        .atten = ADC_ATTEN,
    };
    // Assign channels to each io in adc_io
    for (int i = 0; i < sizeof(adc_io) / sizeof(adc_io[0]); i++)
    {
        adc_channel_t channel;
        adc_unit_t unit;
        ESP_ERROR_CHECK(adc_oneshot_io_to_channel(adc_io[i], &unit, &channel));
        ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_obj->adc1_handle, channel, &config));
    }

    return ESP_OK;
}

static bool adc_calibration_init(adc_unit_t unit, adc_channel_t channel, adc_atten_t atten, adc_cali_handle_t *out_handle)
{
    adc_cali_handle_t handle = NULL;
    esp_err_t ret = ESP_FAIL;
    bool calibrated = false;

#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
    if (!calibrated) {
        ESP_LOGI(TAG, "calibration scheme version is %s", "Curve Fitting");
        adc_cali_curve_fitting_config_t cali_config = {
            .unit_id = unit,
            .chan = channel,
            .atten = atten,
            .bitwidth = ADC_BITWIDTH_LEN,
        };
        ret = adc_cali_create_scheme_curve_fitting(&cali_config, &handle);
        if (ret == ESP_OK) {
            calibrated = true;
        }
    }
#endif

#if ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
    if (!calibrated) {
        ESP_LOGI(TAG, "calibration scheme version is %s", "Line Fitting");
        adc_cali_line_fitting_config_t cali_config = {
            .unit_id = unit,
            .atten = atten,
            .bitwidth = ADC_BITWIDTH_LEN,
        };
        ret = adc_cali_create_scheme_line_fitting(&cali_config, &handle);
        if (ret == ESP_OK) {
            calibrated = true;
        }
    }
#endif

    *out_handle = handle;
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "Calibration Success");
    } else if (ret == ESP_ERR_NOT_SUPPORTED || !calibrated) {
        ESP_LOGW(TAG, "eFuse not burnt, skip software calibration");
    } else {
        ESP_LOGE(TAG, "Invalid arg or no memory");
    }

    return calibrated;
}

esp_err_t calib_init(adc_obj_t *adc_obj)
{
    for (int i = 0; i < sizeof(adc_io) / sizeof(adc_io[0]); i++)
    {
        adc_channel_t channel;
        adc_unit_t unit;
        ESP_ERROR_CHECK(adc_oneshot_io_to_channel(adc_io[i], &unit, &channel));
        adc_obj->do_calib[i] = adc_calibration_init(ADC_UNIT_1, channel, ADC_ATTEN, &adc_obj->adc1_cali_handle[i]);
    }
    return ESP_OK;
}

esp_err_t enable_adc1(adc_obj_t** adc_obj)
{
    adc_obj_t *ret = NULL;
    ret = calloc(1, sizeof(adc_obj_t));
    ESP_ERROR_CHECK(ret == NULL ? ESP_ERR_NO_MEM : ESP_OK);
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &ret->adc1_handle));
    config_adc1(ret);
    calib_init(ret);
    *adc_obj = ret;

    return ESP_OK;
}

int read_adc(adc_handle_t adc_handle, int gpio)
{
    int adc_reading = 0;
    int voltage = 0;
    adc_channel_t channel;
    adc_unit_t unit;
    ESP_ERROR_CHECK(adc_oneshot_io_to_channel(gpio, &unit, &channel));
    ESP_ERROR_CHECK(adc_oneshot_read(adc_handle->adc1_handle, channel, &adc_reading));
    int arr_loc = 0;
    switch (gpio)
    {
        case LSA_A0:
            arr_loc = 0;
            break;
        case LSA_A1:
            arr_loc = 1;
            break;
        case LSA_A2:
            arr_loc = 2;
            break;
        case LSA_A3:
            arr_loc = 3;
            break;
        case LSA_A4:
            arr_loc = 4;
            break;
        default:
            break;
    }
    if (adc_handle->do_calib[arr_loc])
    {
        ESP_ERROR_CHECK(adc_cali_raw_to_voltage(adc_handle->adc1_cali_handle[arr_loc], adc_reading, &voltage));
        return voltage;
    }
    else
    {
        return adc_reading;
    }
}
