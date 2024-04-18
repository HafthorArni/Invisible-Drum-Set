This is set up as a platformIO project in VScode

Upload the C code to the Arduino Nano using platformIO in VScode. Then run the play_sound.py file to listen to the output.
The custom C libraries are stored in lib/src. A lot of these libraries can be improved or substituded for better libraries but they currently work good enough running on my exact hardware (Atmega328P new bootloader). It was a requirement for the course to write all libraries from scratch. Otherwise I would have used some standard libraries such as "util/delay.h" for example.
.
.
Collect_data.c under MAINS is used along side the collect_data.py program to write IMU data to a .txt file for analysis. The matlab script plots the content of the txt file. This could also be plotted using python but I haven't written the code for that.

The C++ code under CPP is really underdeveloped because I made the switch to C early on in the project but it includes the basics to get started if you dont't want to work in C.

Schematic diagram:
![AirDrums_schem](https://github.com/HafthorArni/Invisible-Drum-Set/assets/111596842/338ac0de-1c2d-438f-a483-633e99d87779)
