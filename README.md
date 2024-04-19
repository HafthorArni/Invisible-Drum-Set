# Invisible Drum Set

## Introduction
This project is a simulation of a drum set using an Arduino Nano and the BNO055 IMU sensor. It allows users to play drums without a physical drum set. The project is primarily developed in C for a course requirement that emphasized writing all libraries from scratch.
![drumsticks](https://github.com/HafthorArni/Invisible-Drum-Set/assets/111596842/df8816de-3efb-4c85-85ff-45e4e730acf8)


## Setup
### Prerequisites
- PlatformIO in VScode (or whatever you like)
- Arduino Nano with Atmega328P (new bootloader)
- BNO055 IMU

### Installation
1. Clone the repository.
2. In VScode with PlatformIO, upload the C code to the Arduino Nano.

## Usage
- After uploading `main.c`, execute the `play_sound.py` to get audio output.
- To calibrate, swing the drumsticks in a figure eight motion and then press the button with the drumsticks pointing away from you.
- The `collect_data.c` under MAINS pairs with `collect_data.py` to log IMU data to a .txt file for analysis. The `plot3.m` matlab file plots the data but you could use whatever program you like.
- The project also includes foundational C++ code under CPP but this code is really bad in its current state. I began testing the hardware in C++ before refining the method of simulating the drum set in C.

## Further Development
- Enhancements or replacement of the custom libraries I created could improve the project's maintainability.
- The latency can be decreased. The python program playing the samples can probably be improved.
- The conditional logic for determining which sound to play in “main.c” can be fine-tuned and optimized. Currently it is tuned to my left handed playing style.
- Velocity sensitivity can be added but it requires changing the way communication is handled between the arduino and the python program.
- Error handling.

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
