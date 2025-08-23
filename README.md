# DOOM 1993 Bare Metal Port onto RISC-V MangoPi

This is a bare metal port of the original 1993 DOOM video game onto the MangoPi board (model MPi-MQ1PH). The primary backend dependecies (i.e. C libaries) aren't publicly available as they are content for Stanford's CS107E course. However, many additional functions were implemented and added to the repo via the `pseudo_includes` folder. Due to the bare metal nature of the port, all file/system calls were removed and replaced with memory calls to a C-array storing the original DOOM's `.wad` file. Thus, since there is no SD card support outside of bootloading off of it, games are unable to be saved/loaded each time the MangoPi turns on. 

The main purpose of the port is to display the MangoPi's various protocol support (and of course to show that DOOM can run on anything...). The port utilizes an I2C Adafruit Joy Bonnet for joystick and button input, an SPI  LCD display for video output, and an I2S mono speaker to play the DOOM soundtrack on loop (no native game sound support). In addition to the SPI LCD, there is a "Nintendo Switch"-esque feature where if an HDMI cable is plugged into the MangoPi, it will automatically switch display outputs from the SPI LCD to whatever monitor it is connected to.

## Media
![](screenshots/.png)
