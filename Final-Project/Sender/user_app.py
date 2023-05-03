import serial
import sys
import time
arduino = serial.Serial(port='/dev/cu.usbserial-0001', baudrate=115200, timeout=.1)

def write_read(x):
    arduino.write(bytes(x))
    time.sleep(0.05)
while True:
    with open ('EncoderOutput.npy', 'rb') as file:
        input("Send?\n") # Taking input from user
        write_read(file.read().strip())
    board_output = "."
    while board_output != "":
        board_output = arduino.readline().decode().strip()
        print(board_output)