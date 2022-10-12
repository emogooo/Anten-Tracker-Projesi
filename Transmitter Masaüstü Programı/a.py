from tabnanny import check
from timeit import repeat
from tkinter import *
from tkinter import messagebox
import time
from turtle import delay

class App():
   def __init__(self) -> None:
      global root
      global portStatusLabel
      global connectButton
      global kosul

      kosul = True
      
      root = Tk()
      root.title("Anten Tracker - Transmitter")
      root.geometry("350x100")
      
      portStatusLabel = Label(root, text = "Port Bilgisi")
      portStatusLabel.grid(row = 0, column = 1, padx=15, pady = 5)
      
      connectButton = Button(root, text="Bağlan", command=self.transmit)
      connectButton.grid(row=1, column=1, padx=75)
    
   def repeat(self):
      if kosul:
         print(int(time.time() % 100))
         root.after(1000, self.repeat)

   def openbut(self):
      connectButton.configure(state=NORMAL)

   def transmit(self):
      connectButton.configure(state=DISABLED)
      root.after(400, self.openbut)
      global kosul
      if kosul:
         kosul = False
         print("false yapıldı.")
      else:
         kosul = True
         print("true yapıldı")
         self.repeat()
      
   def run(self):
      self.repeat()
      root.mainloop()
      
app = App()
app.run()