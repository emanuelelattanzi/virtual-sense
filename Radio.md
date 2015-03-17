# Radio transceiver #

VirtualSense communicates through a Texas Instruments CC2520 RF transceiver, a second-generation ZigBee/IEEE 802.15.4 RF transceiver working in the 2.4 GHz band. CC2520 RF provides several integrated features, including: hardware support for frame handling, data buffering, burst transmissions, data encryption, data authentication, clear channel assessment, link quality indication, frame filtering, standard RX mode, and low-current RX mode. All these built-in features significantly reduce the MCU computational load and power consumption.

## Radio-MCU Communication ##

The CC2520 is controlled by the MCU through the SPI port and six general purpose I/O pins (GPIOs). GPIOs are fully programmable and can be configured to route different state signals and exceptions. Notice that, in the proposed architecture, CC2520 GPIOs are fully mapped on the P1 port of the MCU which provides interrupt capabilities. In this way it is possible to raise interrupts associated with specific RF events.

## Power Modes ##


CC2520 provides three power modes (Active, LPM1, and LPM2) and an extra low-current RX mode. In all these modes the power supply is always provided to the device.
LPM2 is the lowest power consumption mode where the digital voltage regulator is turned off, no clocks are running, no data is retained, and all analog modules are in power down state. In this state the power consumption is about 4.5µA, in spite of the complete inactivity and of the need to reboot the embedded controller at wakeup.
In LPM1 the digital voltage regulator is on but no clocks are running. In this state all data and configurations are retained and the analog modules can be controlled by the MCU. The power consumption is about 1mA.


| **Typical output power (dBm)** | **Typical current consumption (mA)** |
|:-------------------------------|:-------------------------------------|
| 5 | 33.6 |
| 3 | 31.3 |
| 2 | 28.7 |
| 1 | 27.9 |
| 0 | 25.8 |
| -2 | 24.9 |
| -4 | 23.1 |
| -7 | 19.9 |
| -18 | 16.2 |

**Table 2. CC2520 transmission power and current consumption**

In Active mode (AM) the digital voltage regulator is on and the crystal oscillator clock is running. The power consumption is of 2.6mA.
The analog RX/TX module can be activated and de-activated when needed by means of a byte instruction provided by the MCU through the SPI bus. In TX mode the power consumption of the entire CC2520 ranges from 16.2mA @ -18dBm to 33.6mA @ +5dBm, while in standard RX mode the power consumption is 23.3mA. Using the low-current RX mode reduces the power consumption in the receiving phase down to 18.5mA at the expense of a decrease in sensitivity from -98dBm to -50dBm.
Transmission output power can be tuned among nine different power levels affecting the power consumption consequently. Table 2 shows the tunable transmission power levels and the corresponding power consumption.

## Frame Filtering ##


The frame filtering function rejects non-intended frames and it can be configured to reject frames not matching the local PAN id, the local short address, or the local extended address previously stored in CC2520 RAM. When frame filtering is enabled and the filtering algorithm accepts a received frame, an RX\_FRM\_ACCEPTED exception is generated to notify the reception of a new frame to the MCU.
Although the frame filtering mechanism is mainly targeted to support firewall functionality, it is possible to use it to save power. In fact, the RX\_FRM\_ACCEPTED exception is generated immediately after receiving the fields required to determine the frame matching without waiting for the entire frame to be received. In this way it is possible to shut down immediately the receiver in case of a non-intended frame and to avoid processing the rest of the frame and waking up the MCU. This trick is particularly effective in case of large packets.