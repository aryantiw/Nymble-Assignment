import serial
import time

def calculate_speed(start_time, end_time, data_size):
    elapsed_time = end_time - start_time
    speed = (data_size * 8) / elapsed_time
    return speed

def receive_data(serial_port, data_size):
    received_data = ""
    start_time = time.time()

    for _ in range(data_size):
        received_data += serial_port.read().decode('utf-8')
        end_time = time.time()
        speed = calculate_speed(start_time, end_time, len(received_data))
        print(f"Live Transmission Speed: {speed:.2f} bits/second", end='\r')

    print("\nData Received:")
    print(received_data)

# Configure the serial port
serial_port = serial.Serial('COM1', 2400, timeout=1)

# Wait for a moment to stabilize the serial connection
time.sleep(2)

# Receive data from MCU
receive_data(serial_port, data_size=1000)

# Close the serial port
serial_port.close()
