# General description and Key Features #

VirtualSense is an ultra-low power wireless node for use in wireless sensor networks (WSNs) subject to tight power constraints. Thanks to the on board Java compatible virtual machine (VM) it allows programmers to rapidly develop monitoring applications and communication protocols.
VirtualSense makes use of IEEE 802.15.4 wireless transceivers in order to standardize communication and to inter-operate with other existing devices. The set of on board sensors (including humidity, temperature, and light), together with the possibility to easily connect any external sensor/actuator, allows VirtualSense to be used in a wide range of application fields.

In order to promote research and development VirtualSense adopts an open-hardware/open-source model. In particular, it mounts widely available off-the-shelf components and it makes publicly available all PCB schematics. The open-source software stack is based on a modified version of Darjeeling java-compatible VM running on top of Contiki operating system.

| <img src='http://wiki.virtual-sense.googlecode.com/git/ultrasonic_trigger.png' width='400'></img>| <img src='http://wiki.virtual-sense.googlecode.com/git/CPU.jpg' width='700'></img>|
|:-------------------------------------------------------------------------------------------------|:----------------------------------------------------------------------------------|
| **Figure 1: The platform overview** |  |

## Key components and features ##

The key components of VirtualSense 1.0 are listed below:

  * 250kbps 2.4GHz IEEE 802.15.4 Texas Instruments cc2520 Wireless Transceiver
  * 25MHz Texas Instruments MSP430f54xxa microcontroller unit (MCU) with 16k RAM and 128k Flash
  * Integrated Humidity, Temperature, and Light sensors
  * 512K I2C™ Serial EEPROM
  * On-board 48-bit I2C Extended Unique Identifier (EUI-48™)
  * On-board programmable ultra-low-power RTC

The distinguishing features include:
  * Ultra low power consumption (≈0.66µW in hibernation, ≈1.5µW in sleep mode, 10/60mW in send/receive modes, respectively)
  * Compatibility with state-of-the-art energy harvesting modules
  * Fast wakeup from sleep mode (<5μs)
  * Programmable timed wake-up from any low-power mode
  * Sensitivity to asynchronous external events
  * Integrated 12-bit ADC/DAC
  * Integrated Supply Voltage Supervisor (SVS)
  * Integrated DMA Controller
  * USB 2.0 RS232/UART communication with a PC
  * Interoperability with other IEEE 802.15.4 devices
  * Open-source software stack
  * Contiki MAC-layer compatibility
  * Java-compatible run-time environment
  * Easy Over the Air (OTA) programming


## Architecture ##
VirtualSense is made of ultra-low-power components in order to keep the average consumption compatible with state-of-the-art energy harvesters. Figure 2 shows the functional block diagram representing the node architecture. The core is a MCU belonging to the Texas Instrument MSP430F54xxa family. It communicates through I2C™ bus with a Microchip 24AA025E48 Extended Unique Identifier and with a Microchip 24AA512 serial 512K EEPROM. Using the SPI bus, the MCU manages the Texas Instruments CC2520 2.4GHz IEEE 802.15.4 RF transceiver and communicates with the NXP PCF2123 ultra low-power real time clock/calendar.

|<img src='http://wiki.virtual-sense.googlecode.com/git/Functional_block_diagram.png' width='500'></img>|
|:------------------------------------------------------------------------------------------------------|
| Figure 2: The functional block diagram |