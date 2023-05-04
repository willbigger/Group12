import serial
import sys
import time
arduino = serial.Serial(port='/dev/cu.usbserial-0001', baudrate=115200, timeout=.1)

def write_read(x):
    arduino.write(bytes(x))
    time.sleep(0.25)
while True:
    with open ('EncoderOutput.npy', 'rb') as file:
        input("Send?\n") # Taking input from user
        while True:
            piecesOfText = file.read(200)
            if piecesOfText == '':
                break
            piecesOfText = piecesOfText + bytes("]")
            write_read(piecesOfText)                
        
        #write_read(file.read(256))

    board_output = "."
    while board_output != "":
        board_output = arduino.readline().decode().strip()
        print(board_output)