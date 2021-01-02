# sra-board-component

![SRA Board component](https://github.com/SRA-VJTI/sra-board-component/workflows/SRA%20Board%20component/badge.svg)
[![License][license-shield]][license-url]

ESP-IDF Component for [SRA Board](https://github.com/SRA-VJTI/sra-board-hardware-design)

## Contents
- [File Structure](#file-structure)
- [License](#license)

## File Structure
    ├── src/                               
    │   ├── adc.c
    │   ├── bar_graph.c
    │   ├── lsa.c
    │   ├── motor_driver.c
    │   ├── mpu6050.c
    │   ├── servo.c
    │   ├── switches.c
    │   ├── utils.c
    ├── include/
    │   ├── adc.h
    │   ├── bar_graph.h
    │   ├── lsa.h
    │   ├── motor_driver.h
    │   ├── mpu6050.h
    │   ├── servo.h
    │   ├── switches.h
    │   ├── utils.h
    │   ├── pin_defs.h
    │   ├── sra_board.h
    ├── examples/
    │   ├── bar_graph/
    │   ├── lsa/
    │   ├── motor_driver_normal/
    │   ├── motor_driver_parallel/
    │   ├── mpu6050/
    │   ├── servo/
    │   ├── switches/
    ├── CMakeLists.txt
    ├── LICENSE
    ├── Makefile
    ├── README.md                
    ├── components.mk                    

## License
- Distributed under the [MIT License](https://github.com/SRA-VJTI/sra-board-component/blob/main/LICENSE).


[license-shield]: https://img.shields.io/github/license/SRA-VJTI/sra-board-component
[license-url]: https://github.com/SRA-VJTI/sra-board-component/blob/main/LICENSE