<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/SRA-VJTI/sra-board-component/">
    <img src="./assets/logo.png" alt="Logo" >
  </a>

  <h3 align="center">sra-board-component</h3>

  <p align="center">
    An ESP-IDF Component for <a href="https://github.com/SRA-VJTI/sra-board-hardware-design">SRA Board</a>
    <br />
    <a href="https://sravjti.tech/sra-board-component/"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/SRA-VJTI/sra-board-component/tree/main/examples">View Demo</a>
    ·
    <a href="https://github.com/SRA-VJTI/sra-board-component/issues">Report Bug</a>
    ·
    <a href="https://github.com/SRA-VJTI/sra-board-component/issues">Request Feature</a>
    ·
    <a href="https://github.com/SRA-VJTI/sra-board-component/pulls">Send a Pull Request</a>
  </p>
</p>

<p align="center">
  <img src="https://github.com/SRA-VJTI/sra-board-component/workflows/SRA%20Board%20component/badge.svg">
  <a href="https://github.com/SRA-VJTI/sra-board-component/network/members">
    <img src="https://img.shields.io/github/forks/SRA-VJTI/sra-board-component">
  </a>
  <a href="https://github.com/SRA-VJTI/sra-board-component/stargazers">
    <img src="https://img.shields.io/github/stars/SRA-VJTI/sra-board-component">
  </a>
  <a href="https://github.com/SRA-VJTI/sra-board-component/issues">
    <img src="https://img.shields.io/github/issues/SRA-VJTI/sra-board-component">
  </a>
  <a href="https://github.com/SRA-VJTI/sra-board-component/blob/main/LICENSE">
    <img src="https://img.shields.io/github/license/SRA-VJTI/sra-board-component">
  </a>
  <a href="https://linkedin.com/in/sra-vjti">
    <img src="https://img.shields.io/badge/-LinkedIn-black.svg?logo=linkedin&colorB=555">
  </a>
</p>



<!-- TABLE OF CONTENTS -->
## Table of Contents

* [About the Project](#about-the-project)
  * [Built With](#built-with)
* [Getting Started](#getting-started)
  * [Prerequisites](#prerequisites)
  * [Installation](#installation)
* [Usage](#usage)
  * [Examples](#examples)
* [Roadmap](#roadmap)
* [Contributing](#contributing)
* [License](#license)
* [Contact](#contact)



<!-- ABOUT THE PROJECT -->
## About The Project

SRA ESP-IDF component provides an abstraction 

### Built With

* [C](https://en.wikipedia.org/wiki/C_(programming_language))

Toolchain & Supported SDK

* [ESP-IDF v4.4-release](https://github.com/espressif/esp-idf/tree/release/v4.4)

Docs

* [Doxygen](https://www.doxygen.nl/index.html)

<!-- GETTING STARTED -->
## Getting Started

* Hardware Required : [SRA Development Board](https://github.com/SRA-VJTI/sra-board-hardware-design)
* Recommended ESP-IDF [v4.4-release](https://github.com/espressif/esp-idf/tree/release/v4.4) .

_Refer [espressif-docs](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/) for intallation guide_

### Prerequisites

* A stable release of [ESP-IDF](https://github.com/espressif/esp-idf)
* Project Tree containing `components` dir

```
- myProject/
             - CMakeLists.txt
             - sdkconfig
             - components/ - component1/ 
                           - component2/ 
             - main/       - CMakeLists.txt
                           - src.c
             - build/
```

### Installation

```sh
cd <your_esp_idf_project>
mkdir components
cd components
git clone https://github.com/SRA-VJTI/sra-board-component.git
```


<!-- USAGE EXAMPLES -->
## Usage

- Visit [examples](https://github.com/SRA-VJTI/sra-board-component/tree/main/examples)

_For more examples, please refer to the [Documentation](https://sravjti.tech/sra-board-component/)_

### Examples

| Example Link  	|   Description	|
|:-:	            |---	          |
|[Bar Graph](https://github.com/SRA-VJTI/sra-board-component/tree/main/examples/bar_graph)       |  Bar graph led can be used for testing purposes 	|
|  [ Light sensing array](https://github.com/SRA-VJTI/sra-board-component/tree/main/examples/lsa) 	|  LSA aka Light Sensing Array is used mainly in line following bot 	|
|  Motor driver - [normal](https://github.com/SRA-VJTI/sra-board-component/tree/main/examples/motor_driver_normal)- [parallel](https://github.com/SRA-VJTI/sra-board-component/tree/main/examples/motor_driver_parallel) 	|  Example for using two motor drivers present on SRA Board in both parallel and normal mode 	|
|  [MPU6050](https://github.com/SRA-VJTI/sra-board-component/tree/main/examples/mpu6050) 	|  MPU6050 combines 3-axis Gyroscope, 3-axis Accelerometer and Motion Processor all in small package. This example is for reading values from mpu6050 accelerometer	|
|  [Servos](https://github.com/SRA-VJTI/sra-board-component/tree/main/examples/servos) 	|  Servos are used in the locomotion of bots like [Wall-E](https://github.com/SRA-VJTI/Wall-E_v2.1) . This example is for using the servo port on SRA Board to run servos 	|
| [Switches](https://github.com/SRA-VJTI/sra-board-component/tree/main/examples/switches) | Manual Switches provide the traditional comfort to toggle the input variables. This example is for using four user switches present on SRA Board |
| [OLED](https://github.com/SRA-VJTI/sra-board-component/tree/main/examples/oled) | Example for initialising the OLED and diplaying the "Hello World" on the screen |

<!-- ROADMAP -->
## Roadmap

See the [open issues](https://github.com/SRA-VJTI/sra-board-component/issues) for a list of proposed features (and known issues).



<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to be learn, inspire, and create. Any contributions you make are **extremely appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request



<!-- LICENSE -->
## License

* Distributed under the [MIT License](https://github.com/SRA-VJTI/sra-board-component/blob/main/LICENSE).

<!-- CONTACT -->
## Contact

* [@SRAVJTI](https://twitter.com/SRAVJTI) - https://www.sravjti.in/
* [SRA-VJTI Website](https://www.sravjti.in/contact-us)



