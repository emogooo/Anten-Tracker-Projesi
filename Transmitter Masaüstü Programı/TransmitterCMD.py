import serial
import serial.tools.list_ports
import time

ports = list(serial.tools.list_ports.comports())
isCommStarted = False
for port in ports:
    if "CH340" in port.description:
        arduino = serial.Serial(port.name,9600)
        arduino.timeout = 0.1
        isCommStarted = True
isTrackerEnable = True
disableTime = 0
if isCommStarted:
    while True:
        if isTrackerEnable:
            dataFromPython = input("Enter\n")
            arduino.write(dataFromPython.encode())
            startTime = time.time()
            while time.time() - startTime < 0.5:
                bytesToRead = arduino.inWaiting()
                if bytesToRead < 5:
                    continue
                dataFromArduino = arduino.read(bytesToRead)
                dataFromArduino = dataFromArduino.decode()
                
                if "*0#" in dataFromArduino:
                    isTrackerEnable = False
                    disableTime = time.time()
                    break
        else:
            bytesToRead = arduino.inWaiting()
            if bytesToRead < 5:
                continue
            dataFromArduino = arduino.read(bytesToRead)
            dataFromArduino = dataFromArduino.decode()
            print(dataFromArduino)

            if "*1#" in dataFromArduino or time.time() - disableTime > 15:
                isTrackerEnable = True

else:
    print("Aygıt bulunamadı.")