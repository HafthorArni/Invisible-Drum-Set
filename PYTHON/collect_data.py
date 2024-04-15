import serial

# Specify the serial port and baud rate
SERIAL_PORT = 'COM10'  # Update this to your serial port
BAUD_RATE = 115200  # Update this to match your device's baud rate

# Specify the full path to the output file
FILE_PATH = r"C:\Users\arnih\OneDrive\Documents\MATLAB\mech2\data.txt"  # Update this to your desired file path

try:
    # Open the serial port
    with serial.Serial(SERIAL_PORT, BAUD_RATE) as ser, open(FILE_PATH, 'w') as file:
        print(f"Logging data to '{FILE_PATH}'")
        while True:
            if ser.in_waiting > 0:
                # Read the incoming data
                data = ser.readline().decode('utf-8').rstrip()
                
                # Write data to the text file
                file.write(data + '\n')
                file.flush()  # Ensure data is written to the file

except serial.SerialException as e:
    print(f"Error opening serial port {SERIAL_PORT}: {e}")
except KeyboardInterrupt:
    print("Stopped by the user")
