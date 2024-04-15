import serial
import time
from pygame import mixer

# Initialize the mixer
mixer.init()
mixer.set_num_channels(32)  # Adjust the number based on your needs

# Load the sound files into Sound objects
snare = mixer.Sound(r'SAMPLES\SNARES\07_Snare_16_SP.wav')
hihat = mixer.Sound(r'SAMPLES\HIHATS\Boom-Bap Hat CL 53.wav')
high_tom = mixer.Sound(r'SAMPLES\TOMS\TOM.wav')
crash = mixer.Sound(r'SAMPLES\CYMBALS\07_Perc_05_SP.wav')
kick = mixer.Sound(r'SAMPLES\KICKS\Boom-Bap Kick 53.wav')
mid_tom = mixer.Sound(r'SAMPLES\TOMS\Acoustic Mid Tom 06.wav')
ride = mixer.Sound(r'SAMPLES\CYMBALS\MachineRide.wav')
flor_tom = mixer.Sound(r'SAMPLES\TOMS\Tomm1.wav')
# Set the initial volume for each sound object
snare.set_volume(1)   # Adjust as needed
high_tom.set_volume(1)  # Adjust as needed
crash.set_volume(1)  # Adjust as needed
hihat.set_volume(3)  # Adjust as needed
kick.set_volume(1)  # Adjust as needed
mid_tom.set_volume(1)  # Adjust as needed
ride.set_volume(1)  # Adjust as needed


# Serial port setup
ser = serial.Serial('COM10', 115200)

while True:
    try:
        if ser.in_waiting > 0:  # Check if data is available
            message = ser.read(ser.in_waiting).decode('utf-8')  # Read all available data
            for char in message:
                if char == '0':
                    snare.play()
                elif char == '1':
                    hihat.play()
                elif char == '2':
                    kick.play()
                elif char == '3':
                    high_tom.play()
                elif char == '4':
                    mid_tom.play()
                elif char == '5':
                    crash.play()
                elif char == '6':
                    ride.play()
                elif char == '7':
                    flor_tom.play()
                    
    except Exception as e:
        print(f"Error: {e}")
