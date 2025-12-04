from gui import GUI
from basics.serial_wrapper import *

if __name__ == '__main__':
    ser = SerialWrapper()
    gui = GUI(ser)
    gui.start()
    print("Done")
