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

#ifndef STEPPER_H
#define STEPPER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <driver/gpio.h>
#include <esp_timer.h>
#include <esp32/rom/ets_sys.h>
#include <esp_log.h>
#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_sleep.h>
#include "pin_defs.h"

#define MIN_STEP_INTERVAL 1000

typedef struct
{
    int step_pin;             // Stepping Signal Pin of ESP32.
    int dir_pin;              // Direction signal Pin of ESP32.
    int min_step_interval_us; // minimum step interval in microseconds
    int steps_per_revolution; // Number of Step pulses per revolution.
    int prev_delay;           // Internal varaible to store previous pulse interval.
    int dir;                  // Internal variable to store the direction of stepper.
    float accl_decel;         // Accelaration/Decelaration value.
    bool movement_done;       // To check if movement is done.
    long current_step_count;  // Internal variable to store current position of stepper motor.
} stepper_config_t;

/** @struct stepper_config_t
 *  @brief This structure contains the configuration of stepper.
 *  @var stepper_config_t::step_pin
 *  Member 'step_pin' contains the gpio pin number to which step signal pin of stepper motor driver is connected
 *  @var stepper_config_t::dir_pin
 *  Member 'dir_pin' contains the gpio pin number to which direction signal pin of stepper motor driver is connected
 *  @var stepper_config_t::min_step_interval_us
 *  Member 'min_step_interval_us' contains the minimum pulse interval for the step signal of stepper motor
 *  @var stepper_config_t::steps_per_revolution
 *  Member 'steps_per_revolution' contains number of step pulses per revolution.
 *  @var stepper_config_t::prev_delay
 *  Member 'prev_delay' contains internal varaible to store previous pulse interval.
 *  @var stepper_config_t::dir
 *  Member 'dir' contains internal variable to store the direction of stepper.
 *  @var stepper_config_t::accl_decel
 *  Member 'accl_decel' contains accelaration/decelaration value.
 *  @var stepper_config_t::movement_done
 *  Member 'movement_done' contains variable to check if movement is done.
 *  @var stepper_config_t::current_step_count
 *  Member 'current_step_count' contains internal variable to store current position of stepper motor.
 */

/**
 * @brief Initiates and configures ESP32 for stepper control

 * @param stepper pointer to stepper_config_t struct

 * @return esp_err_t i.e it shows if pins are configured successfully or not.
 **/
esp_err_t init_stepper(stepper_config_t *stepper);

/**
 * @brief calculates first pulse interval delay as per the accelaration value.

 * @param stepper pointer to stepper_config_t struct

 * @return int containing first pulse interval delay as per the accelaration value.
 **/
int calc_first_delay_in_millis(stepper_config_t *stepper);

/**
 * @brief calculates delay for next step interval for accelaration.

 * @param stepper pointer to stepper_config_t struct

 * @return int containing delay for next step interval for accelaration.
 **/
int cal_delay_accl(stepper_config_t *stepper, int n);

/**
 * @brief calculates delay for next step interval for decelaration.

 * @param stepper pointer to stepper_config_t struct

 * @return int containing delay for next step interval for decelaration.
 **/
int cal_delay_decel(stepper_config_t *stepper, int n);

/**
 * @brief moves stepper to a specified position as per number of steps.

 * @param stepper pointer to stepper_config_t struct.
 * @param pos_step step position to which stepper should be moved.

 * @return esp_err_t i.e it shows if stepper has moved successfully or not.
 **/
esp_err_t move_to_step(stepper_config_t *stepper, float pos_step);

#endif