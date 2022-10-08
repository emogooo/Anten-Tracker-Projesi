from tkinter import *
from tkinter import messagebox
import time

class App():
    def __init__(self) -> None:
        global root
        global portStatusLabel
        global xLabel
        global yLabel
        global xTB
        global yTB
        global transmitButton
        global connectButton
        
        root = Tk()
        root.title("Anten Tracker - Transmitter")
        root.geometry("350x100")
        
        portStatusLabel = Label(root, text = "Port Bilgisi")
        xLabel = Label(root, text = "X Ekseni")
        yLabel = Label(root, text = "Y Ekseni")
        
        portStatusLabel.grid(row = 0, column = 1, padx=15, pady = 5)
        
        xTB = Entry(root, width=10)
        yTB = Entry(root, width=10)

        transmitButton = Button(root, text="Gönder", command=lambda: self.transmit(xTB.get(), yTB.get()))

        connectButton = Button(root, text="Bağlan", command=self.connect)
        connectButton.grid(row=1, column=1, padx=75)
    
    def transmit(self, x, y):
        try:
            if int(x) < 0 or int(x) > 309:
                messagebox.showerror("Hata", "X değeri 0-309 değerleri arasında olmalıdır.")
                return
            
            if int(y) < 0 or int(y) > 124:
                messagebox.showerror("Hata", "Y değeri 0-124 değerleri arasında olmalıdır.")
                return

            arduino.write(('#' + x + '.' + y + '*').encode())
            time.sleep(0.05)
            
        except ValueError:
            messagebox.showerror("Hata", "Girdiler sadece rakamlardan oluşmalıdır.")
            
        except:
            messagebox.showerror("Hata", "Yanlış port veya gönderim hatası.")
       
    def connect(self):
        import serial.tools.list_ports
        ports = list(serial.tools.list_ports.comports())
        for port in ports:
            if "CH340" in port.description:
                global arduino
                arduino = serial.Serial(port.name,9600)
                arduino.timeout = 0.1
                portStatusLabel.config(text= port.name + " Bağlandı.")
                connectButton.grid_forget()
                transmitButton.grid(row=1, column=2, padx=75)
                xLabel.grid(row = 1, column = 0, padx=15, pady = 5)
                yLabel.grid(row = 2, column = 0, padx=15, pady = 5)
                xTB.grid(row = 1, column=1)
                yTB.grid(row = 2, column=1)
                return
        portStatusLabel.config(text= "Aygıt bulunamadı.")

    def run(self):
        root.mainloop()
        
app = App()
app.run()