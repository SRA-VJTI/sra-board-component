# Light Sensor Array (LSA)  

---

## Table of Contents  
- [Overview](#overview)  
- [Logic Followed](#logic-followed)  
- [Hardware Setup](#hardware-setup)
- [Components of LSA](#components-of-lsa)  
- [Functions of Components](#functions-of-components)  
  - [Infrared Emitter](#infrared-emitter)  
  - [Photo Transistor](#photo-transistor)  
  - [Resistors](#resistors)  
  - [JST Connectors](#jst-connectors)  
- [Working Principle](#working-principle)  
- [Software Usage](#software-usage)  
- [Examples & Running](#examples--running)  
- [Applications](#applications)  

---

## Overview  
The **Light Sensor Array (LSA)** detects white and black surfaces by sensing differences in reflectivity. It sends this data to the microcontroller (ESP32 on the SRA Board) for processing, enabling functionalities such as **line following** and **surface detection**.

This example demonstrates interfacing the LSA module with the SRA Board, reading sensor values, and mapping them for control applications.

- **Actual LSA**
<img src="assets/lsa_physical.png" width="750" height="375" >

- **PCB Model**
<img src="assets/lsa_pcb_image.png" width="750" height="375">

---

## Logic Followed

1. **Absorptivity**: Black surfaces exhibit high absorptivity, hence absorb most of the light emitted by the infrared emitters.

2. **Reflectivity**: White surfaces exhibit high reflectivity, hence reflect most of the light back towards the photo transistors.

3. **Sensor Response**: The phototransistors convert the reflected light into electrical signals — higher values for white surfaces and lower values for black surfaces.

4. **Line detection**: The LSA distinguishes lines or edges by detecting variations in reflected light intensity for [applications](#applications).

---

## Hardware Setup  
- Connect the LSA module to the **dedicated sensor port** on the SRA Board.  
- Ensure correct connections for **VCC**, **GND**, and **Signal** lines.  
- Onboard resistors protect both the **infrared emitters** and **photo transistors**.  
- The **JST connector** ensures a compact and reliable connection between the module and the board.

---

## Components of LSA  

| Component | Description | Quantity |
|------------|-------------|-----------|
| Infrared Emitter | Emits infrared light towards the surface | 5 |
| Photo Transistor | Detects reflected infrared light | 5 |
| Resistors | Current limiting for emitters/transistors | 10 |
| JST Connector | Connects LSA module to SRA Board | 1 |

---

## Functions of Components  

### Infrared Emitter  
Emits IR light onto the surface. The light reflected from the surface is then detected by the corresponding photo transistor.  

### Photo Transistor  
Receives the reflected IR light.  
**White surface:** high reflection → high signal value  
**Black surface:** low reflection → low signal value  

### Resistors  
Used to limit the current passing through the emitters and transistors, protecting them from overcurrent.  

### JST Connectors  
Provide a secure and compact interface between the LSA module and the main PCB or microcontroller board.  

---

## Working Principle  
The **LSA** operates based on differences in **reflectivity** of surfaces:

| Surface | Absorptivity | Reflectivity | Output Reading |
|----------|---------------|---------------|----------------|
| Black | High | Low | Low value |
| White | Low | High | High value |

The LSA uses this contrast to detect lines and positions.  

**Example:**  
If the center sensor detects black while side sensors detect white, the system can infer that the line lies beneath the center sensor.

---

## Software Usage  

The SRA Board component provides built-in functions for working with the LSA module.  
These can be found in the `include/` directory under `lsa.h`.

### Function Descriptions  

#### ```esp_err_t enable_line_sensor(int mode)```  
**Description:** Enables and configures the ADC (Analog to Digital Converter) for reading the LSA sensors.  
**Returns:** `ESP_OK` if initialization succeeds, otherwise returns an error (e.g., `ESP_ERR_INVALID_ARG`).

---

#### ```float bound(float val, float min, float max)```  
**Description:** Restricts the given value `val` within the range `[min, max]`.  
**Parameters:**  
- `val` : The value to bound  
- `min` : Minimum limit  
- `max` : Maximum limit  
**Returns:** Bounded value.

---

#### ```float map(float val, float input_lower_limit, float input_higher_limit, float output_lower_limit, float output_higher_limit)```  
**Description:** Maps a number from one range to another.  
**Parameters:**  
- `val`: Value to map  
- `input_lower_limit`: Minimum input range  
- `input_higher_limit`: Maximum input range  
- `output_lower_limit`: Minimum output range  
- `output_higher_limit`: Maximum output range  
**Returns:** Mapped value in the output range.

---

#### ```void vTaskDelay(const TickType_t xTicksToDelay)```  
**Description:** Delays a FreeRTOS task for a specified number of ticks.  
**Parameter:**  
- `xTicksToDelay` : Time in ticks to block the calling task.  

---

## Examples & Running

1. Navigate to the example directory:  
   ```bash
   cd examples/lsa
   ```
2. Set up the ESP-IDF environment:
   ```bash
   source $IDF_PATH/export.sh
   ```
3. Build the project:
   ```bash
   idf.py build
   ```
4. Flash to your ESP32 device and monitor output:
   ```bash
   idf.py flash monitor
   ```

---

## Applications
- Line Following Robots:
Detect and follow black or white lines on a contrasting surface.
- Surface Contrast Detection:
Identify different colors or materials based on light reflectivity.
- Edge Detection:
Detect edges or gaps for navigation or path correction.
- Educational Robotics:
Demonstrate reflectivity principles and sensor interfacing.
- Obstacle Detection (Limited):
Detect sudden changes in reflection levels on flat surfaces.