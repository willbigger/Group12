import serial
import time
arduino = serial.Serial(port='/dev/cu.usbserial-0001', baudrate=115200, timeout=.1)

def write_read(x):
    arduino.write(bytes(x, 'utf-8'))
    time.sleep(0.05)
while True:
    print("Press send:____ to send a message\nPress SF:______ to change the spread factor (integer)")
    usr_input = input("\n") # Taking input from user
    if usr_input[:5] == "send:":
        arduino.write(bytes("0", 'utf-8'))
        with open ('packet.txt', 'r') as file:
            write_read(file.read())
            board_output = "."
            while board_output != "":
                board_output = arduino.readline().decode().strip()
                print(board_output)
    elif usr_input[:3] == "SF:":
        arduino.write(bytes("1", 'utf-8'))
        arduino.write(bytes(str(usr_input[3:]), 'utf-8'))
    else:
        print("\n\nInvalid input")