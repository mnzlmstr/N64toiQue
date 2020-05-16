### What is this Project about ?

In the early 2000s Nintendo released a version of the N64 Console for the Chinese Market called the iQue Player. This Console is much looked after and increasingly hard to come by.
This is made worse by the fact that the inputs, such as the Buttons and the Joystick are hardwired to the Console. As my console began to wear down I sought a way to be able to connect a standart N64 controller to the console. As this was not done yet I created this repository to aid people seeking to do something similar in collecting all the scarce information in one place

### How far along is the Project ?

The current code is working as intended and I played through the first Dungeons of Legen of Zelda - Ocarina of Time without any problems. However the Project is still a mess of wires, and not all Buttons have been hooked up yet. I intend to create a PCB to mount everything back into the origin casing and upload the Eagle Files to this repository, as well as to create assembly instructions and maybe also upload precompiled binaries.

While the first PRB Revision appears to be working right now i will look into improving the PCB to allow <br/>
the User to see if everything is in owrking Order by incorporating LEDs in the PCB as well as a header for 
Programming, which as of right now has to be done beforehand.

### What Hardware is needed for the Project ?

The code is designed to be run on a ESP32 Microprocessor from Espressif running @ 240Mhz.

### What Software is needed to compile from source ?

The code was compiled using Arduino IDE and version 1.0.4 of the Arduino-ESP32 Core.
As Timing is very critical (a bit read from the controller takes only 4uS !) I decided to not bother with working around the compiler optimizations and decided to disable them alltogether. This means you will have to edit the `platform.txt` file to set optimization level to -O0.

### Will I need to perform manual changes ?

Yes you will have to edit the Pins used in the file `PinMappings.h` to match the Pins you wired everything up with. The Axis pins have to be connected to the correct Wires coming from the Joystick. For that is adviced to cut the Joystick cable in half and connect the X-Axis pins to the pins labeled 1 & 4 of the Joystick PCB and the Y-Axis pins to the pins labeled 5 & 6.

### Changelog:

13.04.2020:<br/>
	- moved source code to `src` folder<br/>
	- added precompiled binaries for Heltec WiFi LoRa 32 V2 to folder `bin`<br/>
	- added first PCB Files (untested, just ordered them today) <br/>
<br/>
15.05.2020: <br/>
	- PCB has been tested an appears to be working just fine<br/>
	- TODO: redesigning PCB to incorporate LEDs for user feedback as well as Programming Header<br/>
<br/> 
