#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sra_board.h"

static const char* TAG = "STEPPER_DEBUG";

stepper_config_t stepper_1 = {
    .step_pin = STEP,
    .dir_pin = DIR,
    .accl_decel = 0.001,
    .min_step_interval_us = MIN_STEP_INTERVAL,
    .steps_per_revolution = 200,
    .current_step_count = 0,
};

void run_motor(void *arg)
{
    while (1)
    {
        ESP_LOGI(TAG, "Inside task");
        move_to_step(&stepper_1, 10000);
        ESP_LOGI(TAG, "Done 1");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        move_to_step(&stepper_1, 0);
        ESP_LOGI(TAG, "Done 2");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    init_stepper(&stepper_1);
    xTaskCreatePinnedToCore(&run_motor, "Homing Task", 4096, NULL, 1, NULL, 0);
}
