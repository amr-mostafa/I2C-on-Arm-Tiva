# I2C Implementation on TivaC kit (TM4C123GXL ARM® Cortex®-M4F Based MCU)
Implementation of I2C on Arm Cortex-M4 based microcontroller. The application is just a testing that the protocol is working properly by addressing a slave on the bus with the address **0x3** and **write** operation for a sequence of characters
![](https://i.ibb.co/0n6T3tw/Using-i2c-master-write-format.png)

## Prerequisites
* You must have gnu gcc compiler to build all the project files.
* Recommended: to download and install [code composer studio](https://software-dl.ti.com/ccs/esd/documents/ccs_downloads.html) as it's the best fit for the TM4C123GXL ARM® Cortex®-M4F Based MCUs.

## Project Structure
Project is statically architectured:
* [MCAL](https://github.com/amr-mostafa/I2C-on-Arm-Tiva/tree/master/mcal) which has these drivers; **System Clock**, **I2C**, **NVIC**, **DIO**, **PORT**, **Interrupts**.
* [Config](https://github.com/amr-mostafa/I2C-on-Arm-Tiva/tree/master/config) which has configuration files for the above drivers.
* [utils](https://github.com/amr-mostafa/I2C-on-Arm-Tiva/tree/master/utils) which has header files for **bit manipulations** and **types definitions**.
* [main](https://github.com/amr-mostafa/I2C-on-Arm-Tiva/blob/master/main.c) which is the application that's just sending some characters to a certain slave on the bus.

## License
This project is licensed under the GNU General Public License.

