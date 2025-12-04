#
# Simpler wrapper for USB UART to ITS board
#
from serial import Serial, EIGHTBITS, PARITY_NONE, STOPBITS_ONE
from serial.tools import list_ports
from GUI.my_msg_box_widget import MyMsgBox
from serial.serialutil import SerialException
from basics.thread_save_flag import ThreadSaveFlag
import threading
import time

HELLO_OUT_CMD = b'H'  # First command after reset
READY_IN_CMD = b'R'  # Ready info from ITS board
GO_OUT_CMD = b'G'  # Info to ITS board that it can ask for data
START_IN_CMD = b'S'  # Request from ITS board: Start sending next file
DATA_IN_CMD = b'D'  # Request from ITS board: Send next data package of current file


class SerialWrapper(Serial):
    def __init__(self, **kwargs):
        Serial.__init__(self)
        self.baudrate = kwargs.pop('baudrate', 115200)
        self.bytesize = kwargs.pop('bytesize', EIGHTBITS)
        self.parity = kwargs.pop('parity', PARITY_NONE)
        self.stopbits = kwargs.pop('stopbits', STOPBITS_ONE)
        self.connected = ThreadSaveFlag(False)
        self.setup_in_process = ThreadSaveFlag(False)
        self.disconnect_in_process = ThreadSaveFlag(False)
        self.push_back_buf = []  # RX push back buffer

    def open(self, dev):
        self.port = dev
        Serial.open(self)
        self.flushInput()
        self.flushOutput()
        self.push_back_buf = []

        # __read_polling implements blocking read via polling
        # such that read can be interrupted by an async exception
        # generated via PyThreadState_SetAsyncExc
    def __read_polling(self, n):
        while self.in_waiting < n:
            time.sleep(0.1)
        return self.read(n)

    def read_cmd(self, expected_cmds={READY_IN_CMD, DATA_IN_CMD, START_IN_CMD}):
        ch = self.__read_polling(1) if 0 == len(self.push_back_buf) else self.push_back_buf.pop()
        if ch not in expected_cmds:
            raise Exception("Received unexpected command." + str(ch))
        return ch

    def push_back(self, elem):
        self.push_back_buf.insert(0, elem)

    def write_cmd(self, cmd):
        if cmd not in {HELLO_OUT_CMD, GO_OUT_CMD}:
            raise Exception("Internal error: wrong cmd sent.")
        self.write(cmd)

    def transmit_buf(self, byte_list):
        if len(byte_list) > 0xFFFF:
            raise Exception("byte_list to long")
        self.write(len(byte_list).to_bytes(2, 'big'))
        self.write(byte_list)

    def __connection_protocol(self, os):
        try:
            self.write_cmd(HELLO_OUT_CMD)
            while 0 == self.in_waiting:
                time.sleep(1)
                os.text_area.output_4_oth_th('Waiting for ITS board.\n')
                self.write_cmd(HELLO_OUT_CMD)
            if 1 != self.in_waiting:
                raise SerialException("Input buffer contains unexpected data.")
            if READY_IN_CMD != self.read_cmd():
                raise SerialException("READY command expected.")
            if 0 != self.in_waiting:
                raise SerialException('Unexpected data in RX buffer.')
            self.write_cmd(GO_OUT_CMD)
            os.text_area.output_4_oth_th('Connection to ITS board established.\n')
            self.connected.set(True)
        except SerialException as e:
            if not self.disconnect_in_process.get():
                os.text_area.output_4_oth_th("Error in ITS board communication protocol: " + str(e) + "\n")
            self.close()
            self.connected.set(False)
        self.setup_in_process.set(False)

    def connect(self, parent_widget, dev, os):
        if self.setup_in_process.set(True):
            MyMsgBox(parent_widget, "Connection process is already running.", title="Error", block=True)
            return
        try:
            self.open(dev)
        except SerialException as e:
            txt = "Failed to open serial connection.\n" + str(e)
            self.close()
            MyMsgBox(parent_widget, txt, title="Error", block=True)
            self.setup_in_process.set(False)
            return
        # Start background thread for setup
        t = threading.Thread(target=self.__connection_protocol, args=(os,))
        t.start()

    def disconnect(self):
        self.disconnect_in_process.set(True)
        self.close()
        # wait until connection process has benn finished
        while self.setup_in_process.get():
            time.sleep(0.01)
        self.connected.set(False)
        self.disconnect_in_process.set(False)

    @staticmethod
    def get_serial_USB_interfaces():
        return [elem.device for elem in list_ports.comports() if elem.vid is not None and elem.pid is not None]
