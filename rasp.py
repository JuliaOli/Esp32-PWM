#from gpiozero import LED, Button, Buzzer
import serial


#/dev/ttyUSB0
serialPort = serial.Serial("/dev/ttyUSB0", 9600, timeout=0.7)

serialPort.open()
print(serialPort)

values = bytearray([4, 9, 62, 144, 56, 30, 147, 3, 210, 89, 111, 78, 184, 151, 17, 129])
#serialPort.write(values)

try:
    while True:
        command = serialPort.read_until('\n', size=None)
        commandString = command.decode('utf-8')
        if len(commandString) > 0:
            print(commandString)