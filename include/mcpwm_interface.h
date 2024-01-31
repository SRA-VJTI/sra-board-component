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

#ifndef MCPWM_INTERFACE_H
#define MCPWM_INTERFACE_H

#include <stdint.h>
#include "esp_err.h"

/**
 *   @brief Motor interface
*/

typedef struct motor_t motor_t;
typedef struct motor_t* motor_handle_t;

struct motor_t {
    /**
     * @brief Enable motor
     * @param motor_handle_t Motor object handle
     * @return
     *     - ESP_OK: Success
     *    - ESP_ERR_INVALID_ARG: Invalid argument
     *   - ESP_FAIL: Failed
     */
    esp_err_t (*enable)(motor_t *motor);
    /**
     * @brief Disable motor
     * @param motor Motor object
     * @return
     *     - ESP_OK: Success
     *    - ESP_ERR_INVALID_ARG: Invalid argument
     *   - ESP_FAIL: Failed
     */
    esp_err_t (*disable)(motor_t *motor);
    /**
     * @brief Set motor speed
     * @param motor Motor object
     * @param speed Speed of motor, 0~100
     * @return
     *     - ESP_OK: Success
     *    - ESP_ERR_INVALID_ARG: Invalid argument
     *   - ESP_FAIL: Failed
     */
    esp_err_t (*set_speed)(motor_t *motor, uint32_t speed);
    /**
     * @brief Set direction of motor to forward
     * @param motor Motor object
     * @return
     *    - ESP_OK: Success
     *  - ESP_ERR_INVALID_ARG: Invalid argument
     * - ESP_FAIL: Failed
     */
    esp_err_t (*forward)(motor_t *motor);
    /**
     * @brief Set direction of motor to backwards
     * @param motor Motor object
     * @return
     *   - ESP_OK: Success
     * - ESP_ERR_INVALID_ARG: Invalid argument
     * - ESP_FAIL: Failed
     */
    esp_err_t (*backwards)(motor_t *motor);
    /**
     * @brief Brake motor
     * @param motor Motor object
     * @return
     *  - ESP_OK: Success
     * - ESP_ERR_INVALID_ARG: Invalid argument
     * - ESP_FAIL: Failed
     */
    esp_err_t (*brake)(motor_t *motor);
    /**
     * @brief Delete motor object
     * @param motor Motor object
     * @return
     *     - ESP_OK: Success
     *    - ESP_ERR_INVALID_ARG: Invalid argument
     *   - ESP_FAIL: Failed
     */
    esp_err_t (*del)(motor_t *motor);
};

/**
 * @brief Motor Configuration
 */
typedef struct {
    uint32_t pwma_gpio_num; /*!< Motor PWM A gpio number */
    uint32_t pwmb_gpio_num; /*!< Motor PWM B gpio number */
    uint32_t pwm_freq_hz;   /*!< PWM frequency, in Hz */
} motor_config_t;

/**
 * @brief Motor MCPWM specific configuration
 */
typedef struct {
    int group_id;           /*!< MCPWM group number */
    uint32_t resolution_hz; /*!< MCPWM timer resolution */
} motor_mcpwm_config_t;

/**
 * @brief Create Motor based on MCPWM peripheral
 *
 * @param motor_config: Motor configuration
 * @param mcpwm_config: MCPWM specific configuration
 * @param ret_motor Returned Motor handle
 * @return
 *      - ESP_OK: Create BDC Motor handle successfully
 *      - ESP_ERR_INVALID_ARG: Create BDC Motor handle failed because of invalid argument
 *      - ESP_ERR_NO_MEM: Create BDC Motor handle failed because of out of memory
 *      - ESP_FAIL: Create BDC Motor handle failed because some other error
 */
esp_err_t motor_new_mcpwm_device(const motor_config_t *motor_config, const motor_mcpwm_config_t *mcpwm_config, motor_t** ret_motor);

#endif