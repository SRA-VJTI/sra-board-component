/*
 * Copyright (c) 2021 Society of Robotics and Automation.
 *
 * This file is part of SRA Board Component 
 * (see https://www.sravjti.in).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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
        return adc1_get_raw(ADC_CHANNEL_0);
    }
    else if (adc_pin == LSA_A1)
    {
        return adc1_get_raw(ADC_CHANNEL_3);
    }
    else if (adc_pin == LSA_A2)
    {
        return adc1_get_raw(ADC_CHANNEL_6);
    }
    else if (adc_pin == LSA_A3)
    {
        return adc1_get_raw(ADC_CHANNEL_7);
    }
    else
    {
        ESP_LOGE(TAG_ADC, "Invalid adc pin");
        return ESP_FAIL;
    }
}