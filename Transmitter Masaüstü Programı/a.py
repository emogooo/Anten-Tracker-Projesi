from tkinter import *
from tkinter import messagebox
import os
import serial
import time

class App():
    def __init__(self) -> None:
        
        global root
        global portNumLabel
        global xLabel
        global yLabel
        global portNumTB
        global xTB
        global yTB
        global transmitButton
        
        root = Tk()
        root.title("Anten Tracker - Transmitter")
        root.geometry("350x100")
        
        portNumLabel = Label(root, text = "Port Numarası")
        xLabel = Label(root, text = "X Ekseni")
        yLabel = Label(root, text = "Y Ekseni")
        
        portNumLabel.grid(row = 0, column = 0, padx=15, pady = 5)
        xLabel.grid(row = 1, column = 0, padx=15, pady = 5)
        yLabel.grid(row = 2, column = 0, padx=15, pady = 5)
        
        portNumTB = Entry(root, width=10)
        xTB = Entry(root, width=10)
        yTB = Entry(root, width=10)
        
        portNumTB.grid(row = 0, column=1)
        xTB.grid(row = 1, column=1)
        yTB.grid(row = 2, column=1)
        
        transmitButton = Button(root, text="Gönder", command=lambda: self.transmit(portNumTB.get(), xTB.get(), yTB.get()))
        transmitButton.grid(row=1, column=2, padx=75)
    
    def transmit(self, port, x, y):
        try:
            if int(port) < 0 or int(port) > 255:
                messagebox.showerror("Hata", "Port değer 0-255 değerleri arasında olmalıdır.")
                return
            
            if int(x) < 0 or int(x) > 309:
                messagebox.showerror("Hata", "X değeri 0-309 değerleri arasında olmalıdır.")
                return
            
            if int(y) < 0 or int(y) > 124:
                messagebox.showerror("Hata", "Y değeri 0-124 değerleri arasında olmalıdır.")
                return
            
            arduino = serial.Serial(port='COM' + port, baudrate=9600)
            arduino.write(bytes(x+'.'+y, 'utf-8'))
            time.sleep(0.05)
            data = arduino.readline()
            print(data)
            
        except ValueError:
            messagebox.showerror("Hata", "Girdiler sadece rakamlardan oluşmalıdır.")
            
        except:
            messagebox.showerror("Hata", "Yanlış port veya gönderim hatası.")
       
    
    def run(self):
        root.mainloop()
        
app = App()
app.run()