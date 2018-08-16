Description

An E-ink module is great, how about to make it even nicer? Here we go! Instead of using SPI protocol, we created this I2C E-ink display to join our Qwiic family. So you can just daisy chain this E-ink display just like the other Qwiic modules!

Nothing fancy here, we add a STM32 as the gateway, it can communicate with I2C system, then drive E-ink display to refresh content by SPI protocol.

In order to inherit the ultra low power consuming feature of the E-ink display, we let the STM32 IC go into deep sleep mode (2uA power consuming) and add an I2C chip that is used for waking STM32 up.


Product Buy Link: https://www.smart-prototyping.com/Prototyping-Modules/zio/%20Zio-Qwiic-E-ink-Display-I2C
