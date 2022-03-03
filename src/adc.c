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

static const char *TAG_ADC = "adc";

esp_err_t config_adc1()
{
    // Configure ADC to 12 bit width
    CHECK(adc1_config_width(ADC_WIDTH_BIT_12));

    // Configure ADC to 11dB attenuation
    CHECK(adc1_config_channel_atten(ADC_CHANNEL_0, ADC_ATTEN_DB_11));
    CHECK(adc1_config_channel_atten(ADC_CHANNEL_3, ADC_ATTEN_DB_11));
    CHECK(adc1_config_channel_atten(ADC_CHANNEL_6, ADC_ATTEN_DB_11));
    CHECK(adc1_config_channel_atten(ADC_CHANNEL_7, ADC_ATTEN_DB_11));

    ESP_LOGI(TAG_ADC, "Configured ADC_1 to 12 Bit and 11dB attenuation");

    return ESP_OK;
}

esp_err_t characterize_adc1()
{
    esp_adc_cal_characteristics_t adc_chars;
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, DEFAULT_VREF, &adc_chars);

    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP)
    {
        ESP_LOGI(TAG_ADC, "eFuse Two Point: Supported");
        ESP_LOGI(TAG_ADC, "Characterized using Two Point Value");
    }
    else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF)
    {
        ESP_LOGI(TAG_ADC, "eFuse Vref: Supported");
        ESP_LOGI(TAG_ADC, "Characterized using eFuse Vref");
    }
    else
    {
        ESP_LOGI(TAG_ADC, "Characterized using Default Vref");
    }
    return ESP_OK;
}

esp_err_t enable_adc1()
{
    CHECK(config_adc1());
    CHECK(characterize_adc1());

    ESP_LOGI(TAG_ADC, "Configured and Characterized adc 1");

    return ESP_OK;
}

int read_adc(int adc_pin)
{
    if (adc_pin == LSA_A0)
    {
        return adc1_get_raw(ADC_CHANNEL_6);
    }
    else if (adc_pin == LSA_A1)
    {
        return adc1_get_raw(ADC_CHANNEL_7);
    }
    else if (adc_pin == LSA_A2)
    {
        return adc1_get_raw(ADC_CHANNEL_0);
    }
    else if (adc_pin == LSA_A3)
    {
        return adc1_get_raw(ADC_CHANNEL_3);
    }
    else
    {
        ESP_LOGE(TAG_ADC, "Invalid adc pin");
        return ESP_FAIL;
    }
}
