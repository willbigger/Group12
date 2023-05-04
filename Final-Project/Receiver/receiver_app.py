import serial
import sys
import time
arduino = serial.Serial(port='/dev/cu.usbserial-21', baudrate=115200, timeout=.1)

while True:
    file = open('received_packet.txt', 'a')
    board_output = arduino.readline()
    while board_output != b'':
        file.write(board_output.decode()[1:])
        board_output = arduino.readline()
    file.close()
    time.sleep(0.05)