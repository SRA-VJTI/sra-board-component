#include "adc.h"

static const char* TAG_ADC= "adc";
static esp_adc_cal_characteristics_t *adc_chars;

esp_err_t config_adc1(int channel[])
{   
    // Configure ADC to 12 bit width
    esp_err_t err = adc1_config_width(ADC_WIDTH_BIT_12); 

    if(err == ESP_OK)
    {
        // Configure ADC to 11dB attenuation
        for (int i = 0; i < 4; i++)
        {   
            switch(channel[i])
            {
                case LSA_A0:
                    err = adc1_config_channel_atten(ADC_CHANNEL_0, ADC_ATTEN_DB_11);
                    break;
                case LSA_A1:
                    err = adc1_config_channel_atten(ADC_CHANNEL_3, ADC_ATTEN_DB_11);
                    break;
                case LSA_A2:
                    err = adc1_config_channel_atten(ADC_CHANNEL_6, ADC_ATTEN_DB_11);
                    break;
                case LSA_A3:
                    err = adc1_config_channel_atten(ADC_CHANNEL_7, ADC_ATTEN_DB_11);
                    break;
                default:
                    ESP_LOGE(TAG_ADC, "LSA GPIO pins not passed to the function");
                    return ESP_FAIL;
            }
        }
    }
    else
    {
        return ESP_FAIL;
    }
    
    if (err == ESP_OK )
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

esp_err_t enable_adc1(int channel[])
{
    esp_err_t err = config_adc1(channel);

    if(err == ESP_OK)
    {
        err = char_adc1();
    }
    else
    {
        return ESP_FAIL;
    }
    
    if (err == ESP_OK)
    {
        ESP_LOGI(TAG_ADC, "Configured and Characterized adc 1");
        return ESP_OK;
    }
    else
    {
        return ESP_FAIL;
    }
}

esp_err_t read_adc(int adc_pin)
{
    switch(adc_pin)
    {
        case LSA_A0:
            return adc1_get_raw(ADC_CHANNEL_0);
        case LSA_A1:
            return adc1_get_raw(ADC_CHANNEL_3);
        case LSA_A2:
            return adc1_get_raw(ADC_CHANNEL_6);
        case LSA_A3:
            return adc1_get_raw(ADC_CHANNEL_7);
        default:
            ESP_LOGE(TAG_ADC, "LSA GPIO pins not passed to function");
            return ESP_FAIL;
    }
}