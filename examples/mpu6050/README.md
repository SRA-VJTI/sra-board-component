# MPU6050 6-Axis Motion Sensor Driver for ESP32

A comprehensive ESP-IDF driver for the MPU6050 IMU (Inertial Measurement Unit) featuring complementary filter fusion and automatic calibration.

## Table of Contents
- [What is MPU6050?](#what-is-mpu6050)
- [How IMUs Work](#how-imus-work)
- [Hardware Overview](#hardware-overview)
- [Function Declarations](#function-declarations)
- [Code Workflow](#code-workflow)
- [Theory Behind the Code](#theory-behind-the-code)

---

## What is MPU6050?

The MPU6050 is a 6-axis Motion Processing Unit (MPU) that combines:
- **3-axis Accelerometer** - Measures linear acceleration (including gravity)
- **3-axis Gyroscope** - Measures angular velocity (rotation rate)

### Key Specifications
- **I2C Interface** - Communicates via I2C protocol (address: 0x68 or 0x69)
- **16-bit ADC** - High-resolution sensor readings
- **Configurable Ranges**:
  - Accelerometer: ±2g, ±4g, ±8g, ±16g
  - Gyroscope: ±250°/s, ±500°/s, ±1000°/s, ±2000°/s
- **Low Power** - Built-in power management
- **Applications** - Drones, self-balancing robots, motion tracking, gesture recognition

---

## How IMUs Work

### Accelerometer
Measures proper acceleration (not coordinate acceleration). When stationary, it primarily senses gravity:

```
If device is flat: az ≈ 1g (9.8 m/s²)
If tilted 45°: ax and az ≈ 0.707g each
```

**Pros**: Accurate long-term, senses absolute orientation relative to gravity  
**Cons**: Noisy, affected by vibrations and external acceleration

### Gyroscope
Measures angular velocity (rate of rotation):

```
If rotating at 50°/s for 0.1s → rotated 5°
```

**Pros**: Smooth, not affected by linear motion  
**Cons**: Drifts over time, gives relative change (needs integration)

### The Problem
- **Accelerometer alone**: Too noisy for smooth control
- **Gyroscope alone**: Drifts away from true orientation

### The Solution: Complementary Filter
Fuses both sensors using weighted average:

```
angle = 0.995 × (angle + gyro_rate × dt) + 0.005 × accel_angle
        └─────────── smooth ───────────┘   └── corrects drift ──┘
```

Trust gyro 99.5% for smoothness, use accel 0.5% to prevent drift.

---

## Hardware Overview

### MPU6050 Internal Architecture

```
┌─────────────────────────────────────┐
│          MPU6050 Chip               │
│                                     │
│  ┌──────────────┐  ┌─────────────┐  │
│  │ Accelerometer│  │  Gyroscope  │  │
│  │  (3-axis)    │  │  (3-axis)   │  │
│  │   MEMS       │  │   MEMS      │  │
│  └──────┬───────┘  └──────┬──────┘  │
│         │                 │         │
│         └─────┬───────────┘         │
│               ↓                     │
│       ┌──────────────┐              │
│       │   16-bit ADC │              │
│       └──────┬───────┘              │
│              ↓                      │
│       ┌──────────────┐              │
│       │ I2C Interface│─────→ SDA    │
│       │   (0x68)     │─────→ SCL    │
│       └──────────────┘              │
└─────────────────────────────────────┘
```
---

## Function Declarations

The header declares all public functions with detailed documentation:

1. **Initialization Functions**
   - `i2c_master_init()` - Setup I2C bus
   - `enable_mpu6050()` - Configure MPU6050

2. **Low-Level Reading**
   - `mpu6050_read_acce()` - Read raw accelerometer bytes
   - `mpu6050_read_gyro()` - Read raw gyroscope bytes
   - `combine_msb_lsb_raw_data()` - Convert bytes to integers

3. **Angle Computation**
   - `compute_acce_angle()` - Calculate tilt from accelerometer
   - `compute_gyro_angle()` - Calculate rotation from gyroscope

4. **High-Level Functions**
   - `read_mpu6050_raw()` - Read both sensors
   - `complementary_filter()` - Fuse sensor data
   - `read_mpu6050()` - Main function (ready to use)

5. **Calibration**
   - `calibrate_mpu6050()` - Auto-calibrate sensor offsets
   - `avg_sensors()` - Helper for averaging readings

---



## Code Workflow

### Initialization

#### `esp_err_t i2c_master_init(void)`
Initializes I2C bus in master mode.

**Returns**: `ESP_OK` on success, error code otherwise

**What it does**:
1. Initializes i2cdev library
2. Configures I2C port, pins, and speed
3. Creates mutex for thread-safe access

---

#### `esp_err_t enable_mpu6050(void)`
Wakes up and configures MPU6050.

**Returns**: `ESP_OK` on success, error code otherwise

**Configuration applied**:
1. Clock source: X-axis gyro PLL (most stable)
2. Gyro range: ±250°/s (high precision)
3. Accel range: ±2g (high precision)
4. Disables sleep mode

**Example**:
```c
if (enable_mpu6050() != ESP_OK) {
    ESP_LOGE(TAG, "MPU6050 initialization failed!");
    return;
}
```

---

### Reading Sensors

#### `esp_err_t mpu6050_read_acce(uint8_t *data_rd, size_t size)`
Reads raw accelerometer bytes.

**Parameters**:
- `data_rd`: Buffer to store 6 bytes
- `size`: Should be 6 (or `BUFF_SIZE`)

**Returns**: `ESP_OK` on success

---

#### `esp_err_t mpu6050_read_gyro(uint8_t *data_rd, size_t size)`
Reads raw gyroscope bytes.

**Parameters**: Same as accelerometer

---

#### `esp_err_t read_mpu6050_raw(int16_t *acce_raw_value, int16_t *gyro_raw_value)`
Reads both sensors and converts to 16-bit integers.

**Parameters**:
- `acce_raw_value`: Array[3] for ax, ay, az
- `gyro_raw_value`: Array[3] for gx, gy, gz

**Example**:
```c
int16_t accel[3], gyro[3];
if (read_mpu6050_raw(accel, gyro) == ESP_OK) {
    printf("Accel: %d, %d, %d\n", accel[0], accel[1], accel[2]);
}
```

---

### Angle Calculation

#### `void compute_acce_angle(int16_t ax, int16_t ay, int16_t az, float *acce_angle)`
Calculates roll and pitch from accelerometer.

**Parameters**:
- `ax, ay, az`: Raw accelerometer values
- `acce_angle`: Output array[2] - [roll, pitch] in degrees

**Math**:
```
roll = atan2(ay, √(ax² + az²)) × 57.296°
pitch = atan2(-ax, √(ay² + az²)) × 57.296°
```

---

#### `void compute_gyro_angle(int16_t gx, int16_t gy, int16_t gz, float dt, float *gyro_angle)`
Calculates angle change from gyroscope.

**Parameters**:
- `gx, gy, gz`: Raw gyro values
- `dt`: Time elapsed since last reading (seconds)
- `gyro_angle`: Output array[2] - angular displacement

**Math**:
```
gx_dps = gx / 131          (convert to degrees/second)
angle_change = gx_dps × dt  (degrees rotated)
```

---

### Sensor Fusion

#### `void complementary_filter(int16_t *acce_raw_value, int16_t *gyro_raw_value, float *complementary_angle, float *mpu_offset)`
Fuses accelerometer and gyroscope data.

**Parameters**:
- `acce_raw_value`: Raw accelerometer data
- `gyro_raw_value`: Raw gyroscope data
- `complementary_angle`: Filtered output (updated in place)
- `mpu_offset`: Calibration offset to apply

**Algorithm**:
```
On first call: Initialize angle from accelerometer
On subsequent calls:
    1. Calculate dt (time since last call)
    2. Get angles from both sensors
    3. Apply filter:
       angle = 0.995(angle + gyro_Δ) + 0.005(accel_angle)
```

---

### Main Function

#### `esp_err_t read_mpu6050(float *euler_angle, float *mpu_offset)`
**THE** function to use - handles everything.

**Parameters**:
- `euler_angle`: Output array[2] for roll and pitch (degrees)
- `mpu_offset`: Calibration offset (from `calibrate_mpu6050()`)

**Returns**: `ESP_OK` on success

**Example**:
```c
float angles[2];
float offset[2] = {0, 0};  // Or from calibration

while (1) {
    read_mpu6050(angles, offset);
    printf("Roll: %.2f°, Pitch: %.2f°\n", angles[0], angles[1]);
    vTaskDelay(pdMS_TO_TICKS(10));
}
```

---

### Calibration

#### `esp_err_t calibrate_mpu6050(void)`
Automatically calibrates sensor offsets.

**Returns**: `ESP_OK` if calibrated, `ESP_FAIL` if exceeds attempts

**Requirements**: Device must be stationary and level during calibration!

**Process**:
1. Takes multiple readings
2. Iteratively adjusts offsets to minimize error
3. Gyro target: 0 on all axes
4. Accel target: X=0, Y=0, Z=16384 (1g)
5. Stops when all axes within ±5 units or after 20 attempts

**Example**:
```c
ESP_LOGI(TAG, "Calibrating... keep device still!");
if (calibrate_mpu6050() == ESP_OK) {
    ESP_LOGI(TAG, "Calibration successful!");
} else {
    ESP_LOGW(TAG, "Calibration failed - using defaults");
}
```

**Internal Storage**: Offsets stored in static variables, automatically applied in subsequent reads.

---

#### `esp_err_t avg_sensors(int16_t *acce_raw_value_avg, int16_t *gyro_raw_value_avg, const int16_t *acce_offs, const int16_t *gyro_offs)`
Helper function for calibration - averages multiple readings.

**Not typically called directly by users.**

---

## Theory Behind the Code

### Why Complementary Filter?

**Kalman Filter Alternative**: More complex, needs tuning matrices  
**Complementary Filter**: Simple, effective, computationally cheap

**The Math**:
```
α = 0.995 (trust gyro 99.5%)

θ[n] = α(θ[n-1] + ω×dt) + (1-α)θ_accel
       └── prediction ──┘   └─ correction ─┘
```

**Frequency Domain View**:
- **High-pass** filter on gyro (blocks drift)
- **Low-pass** filter on accel (blocks noise)
- Combined = best of both worlds

### Coordinate System

```
    Z↑ (up)
     |
     |___→ X (forward)
    /
   / Y (right)
```

**Euler Angles**:
- **Roll**: Rotation around X-axis (tilt left/right)
- **Pitch**: Rotation around Y-axis (tilt forward/back)
- **Yaw**: Rotation around Z-axis (rotate like compass)

*Note: This driver only calculates roll and pitch, not yaw (requires magnetometer or gyro integration).*

### Sensitivity Values

**Accelerometer (±2g range)**:
- Sensitivity: 16384 LSB/g
- Formula: `accel_g = raw_value / 16384`

**Gyroscope (±250°/s range)**:
- Sensitivity: 131 LSB/(°/s)
- Formula: `gyro_dps = raw_value / 131`

### Why Z-axis Reads 16384 When Still?

When device is flat and stationary:
- X, Y axes: Perpendicular to gravity → 0g
- Z axis: Parallel to gravity → 1g = 16384

This is how we know the device orientation!

### Calibration Math

**Goal**: Find offset such that `(reading - offset) = expected_value`

**Iterative approach**:
```
offset_new = offset_old + (reading - expected)
```

Example for Z-axis accel:
```
Reading: 16500
Expected: 16384
Current offset: 0
New offset: 0 + (16500 - 16384) = 116

Next reading with offset: 16500 - 116 = 16384 ✓
```
---

## Performance Considerations

**Update Rate**: With 100 kHz I2C, reading both sensors takes ~1ms
- Recommended loop rate: 100-200 Hz (10-5ms delay)
- Higher rates = smoother but more CPU usage

**Complementary Filter Alpha**:
- 0.995 = Good balance for 100 Hz
- Adjust if using different update rates:
  - α = τ / (τ + dt)
  - τ = time constant (~0.5 seconds)
  - dt = loop period

---

