#include "adc.h"

static const char* TAG_ADC= "adc";
static esp_adc_cal_characteristics_t adc_chars;

esp_err_t config_adc1()
{   
    esp_err_t err_A; 
    esp_err_t err_B;

    // Configure ADC to 12 bit width
    err_A = adc1_config_width(ADC_WIDTH_BIT_12); 
    
    // Configure ADC to 11dB attenuation
    err_B += adc1_config_channel_atten(ADC_CHANNEL_0, ADC_ATTEN_DB_11);
    err_B += adc1_config_channel_atten(ADC_CHANNEL_3, ADC_ATTEN_DB_11);
    err_B += adc1_config_channel_atten(ADC_CHANNEL_6, ADC_ATTEN_DB_11);
    err_B += adc1_config_channel_atten(ADC_CHANNEL_7, ADC_ATTEN_DB_11);

    if (err_A == ESP_OK && err_B == ESP_OK)
    {
        ESP_LOGI(TAG_ADC, "Configured ADC_1 to 12 Bit and 11dB attenuation");
        return ESP_OK;
    }
    else
    {
        return ESP_FAIL;
    }
}

esp_err_t characterize_adc1()
{
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
    esp_err_t err_C = config_adc1();
    esp_err_t err_D = characterize_adc1();
    
    if (err_C == ESP_OK && err_D == ESP_OK)
    {
        ESP_LOGI(TAG_ADC, "Configured and Characterized adc 1");
        return ESP_OK;
    }
    else
    {
        return ESP_FAIL;
    }
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
    else if(adc_pin == LSA_A2)
    {
        return adc1_get_raw(ADC_CHANNEL_6);
    }
    else if (adc_pin == LSA_A3)
    {
        return adc1_get_raw(ADC_CHANNEL_7);
    }
    else
    {
        return ESP_FAIL;
    }
}