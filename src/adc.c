#include "adc.h"

static const char* TAG_ADC= "adc";

esp_err_t config_adc1(adc1_channel_t channel[])
{   
    esp_err_t err_A; 
    esp_err_t err_B;

    err_A = adc1_config_width(ADC_WIDTH_BIT_12); //Configure ADC to 12 bit width

	for (int i = 0; i<4; i++)
	{
		err_B = adc1_config_channel_atten(channel[i], ADC_ATTEN_DB_11); //Configure ADC to 11dB attenuation
	}

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

esp_err_t char_adc1()
{
	adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);
       
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