# Importing Libraries
import serial
import serial.tools.list_ports
ports = list(serial.tools.list_ports.comports())
isCommStarted = False
for port in ports:
    if "CH340" in port.description:
        arduino = serial.Serial(port.name,9600)
        arduino.timeout = 1
        isCommStarted = True
if isCommStarted:
    while True:
        num = input("Enter\n") # Taking input from user
        arduino.write(num.encode())
else:
    print("Aygıt bulunamadı.")