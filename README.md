# Invisible Drum Set

## Introduction
This project is a simulation of a drum set using an Arduino Nano and the BNO055 IMU sensor. It allows users to play drums without a physical drum set. The project is primarily developed in C for a course requirement that emphasized writing all libraries from scratch.

## Setup
### Prerequisites
- PlatformIO in VScode (or whatever)
- Arduino Nano with Atmega328P (new bootloader)

### Installation
1. Clone the repository.
2. In VScode with PlatformIO, upload the C code to the Arduino Nano.

## Usage
After uploading `main.c`, execute the `play_sound.py` to get audio from your computer or headphones. Swing the drumsticks in a figure eight motion and then press the button to calibrate.
.
The `collect_data.c` under MAINS pairs with `collect_data.py` to log IMU data to a .txt file for analysis. The matlab file plots the data but you could use whatever program you like.
.
The project also includes foundational C++ code under CPP but this code is really bad in its current state. I began testing the hardware in C++ before refining the method of simulating the drum set in C.

## Further Development
Enhancements or replacement of the custom libraries I created could improve the project's maintainability.
The latency can be decreased. Probably the python program playing the samples can be improved.
The conditional logic for determining which sound to play in “main.c” can be fine-tuned and optimized. Currently it is tuned to my left handed playing style.
Velocity sensitivity can be added but it requires changing the way communication is handled between the arduino and the python program.

## Schematic Diagram
![image (6)](https://github.com/HafthorArni/Invisible-Drum-Set/assets/111596842/853b1885-bde0-454a-a892-8da48c033565)


## Software diagram
![image (7)](https://github.com/HafthorArni/Invisible-Drum-Set/assets/111596842/754ff8bd-817c-479d-af5a-7da1d924d395)


## Contributing
Contributions are welcome! Feel free to fork the project, make changes, and submit a pull request.

## Contact
For queries or collaborations, reach out through GitHub issues or email arnihafthor@gmail.com.

## Documentation
[Link to Project Report](https://drive.google.com/file/d/1G6_Wm_p50Q7FowhHeAtZddCkF3J9PWr1/preview)
