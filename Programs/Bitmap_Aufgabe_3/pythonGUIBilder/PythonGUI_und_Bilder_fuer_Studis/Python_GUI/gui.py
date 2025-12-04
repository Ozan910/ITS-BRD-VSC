import tkinter as tk
import tkinter.ttk as ttk
from GUI.selection_box_widget import SelectBox
from GUI.my_msg_box_widget import MyMsgBox
from GUI.thread_save_scrolled_text import MyScrolledText
from basics.thread_save_flag import ThreadSaveFlag
from basics.my_thread import MyThread
from basics.serial_wrapper import START_IN_CMD, DATA_IN_CMD


class GUI(tk.Tk):
    def __init__(self, uart):
        tk.Tk.__init__(self)
        self.bitmap_list_file = "./BITMAP_list.txt"
        self.height = 450
        self.width = 800
        self.title("Send BitMap Files to ITS board via USB-UART")
        self.configure(background='white')
        self.font = 'Arial 13'
        self.uart = uart
        self.connect_button, self.start_button = None, None
        self.load_files_button, self.disconnect_button, self.text_area = None, None, None
        self.style = ttk.Style()
        self.define_styles()
        self.start_widgets()
        self.file_list = None
        self.send_is_running = ThreadSaveFlag(False)
        self.send_thread = None

    def start(self):
        self.mainloop()

    def start_widgets(self):
        self.load_files_button = ttk.Button(self, text="Load Files", style='FixButton.MyButton.TButton',
                                            command=lambda: self.callback_load_files_button(), takefocus=False)
        self.load_files_button.grid(row=1, column=0, padx=20, pady=10, sticky=tk.W)
        self.connect_button = ttk.Button(self, text="Connect", style='FixButton.MyButton.TButton',
                                         command=lambda: self.callback_connect_button(), takefocus=False)
        self.connect_button.grid(row=2, column=0, padx=20, pady=10, sticky=tk.W)
        self.start_button = ttk.Button(self, text="Start", style='FixButton.MyButton.TButton',
                                       command=lambda: self.callback_start_button(), takefocus=False)
        self.start_button.grid(row=2, column=1, padx=0, pady=10)
        self.disconnect_button = ttk.Button(self, text="Disconnect", style='FixButton.MyButton.TButton',
                                            command=lambda: self.callback_disconnect_button(), takefocus=False)
        self.disconnect_button.grid(row=2, column=2, padx=0, pady=10)
        self.text_area = MyScrolledText(self, width=87, height=12, state='disabled',
                                        fg='black', bg="white", font=self.font)
        self.text_area.focus()
        self.text_area.grid(row=3, column=0, columnspan=5, padx=20, pady=20, sticky=tk.W)

    def send_files(self):
        FILEBUFSIZE, SENDBUFSIZE = 4096, 512
        try:
            for f in self.file_list:
                while DATA_IN_CMD == self.uart.read_cmd({DATA_IN_CMD, START_IN_CMD}):
                    self.uart.transmit_buf([])
                # S command received - open next file
                self.text_area.output_4_oth_th("Sending file " + f + " to ITS board.\n")
                with open(f, "rb") as file:
                    (buf, idx) = ([], 0)
                    while True:
                        (buf, idx) = (file.read(FILEBUFSIZE), 0) if idx >= len(buf) else (buf, idx)
                        self.uart.transmit_buf(buf[idx:idx + SENDBUFSIZE])
                        idx = idx + SENDBUFSIZE
                        if START_IN_CMD == self.uart.read_cmd({DATA_IN_CMD, START_IN_CMD}):
                            self.uart.push_back(START_IN_CMD)
                            break
            self.text_area.output_4_oth_th("All files send.\n")
            self.send_is_running.set(False)
        finally:
            self.text_area.output_4_oth_th("Sending thread terminated.\n")
            self.send_is_running.set(False)

    def callback_start_button(self):
        if not self.uart.connected.get():
            MyMsgBox(self, "Connection to ITS board does not exist.", title="Error", block=True)
            return
        if (self.file_list is None) or (0 == len(self.file_list)):
            MyMsgBox(self, "No BITMAP file selected.\nPlease press Load Files button.", title="Error", block=True)
            return
        if self.send_is_running.set(True):
            MyMsgBox(self, "File transfer process is already running.", title="Error", block=True)
            return
        self.send_thread = MyThread(target=self.send_files, args=())
        self.send_thread.start()

    def callback_disconnect_button(self):
        if self.send_is_running.get():
            # terminate send thread
            self.send_thread.raise_exception()
            self.send_thread.join()
        # disconnect
        self.uart.disconnect()
        self.text_area.output_4_GUI_th("Connection to ITS board canceled.\n")
        self.text_area.output_4_GUI_th("Please reconnect to ITS board.\n")

    def callback_connect_button(self):
        if self.uart.connected.get():
            MyMsgBox(self, "Connection to ITS board already exists.", title="Error", block=True)
            return
        help_text = "List of USB COM ports. Several USB COM ports may be listed.\n" + \
                    "If this list is empty, then ITS board is not connected to your computer.\n" + \
                    "\nThe ITS board and this GUI may be started in arbitrary order, but\n" +\
                    "   - the USB connection to ITS board must not be disconnected\n" + \
                    "   - a restart of the GUI requires a restart of ITS board and vise versa."
        SelectBox(self, title="Select USB COM port for communication with ITS board",
                  button_text="connect", alt_range=self.uart.get_serial_USB_interfaces(),
                  help_text=help_text, action=lambda a1, a2: self.uart.connect(a1, a2, self))

    def set_file_list(self, fl):
        self.file_list = fl
        self.text_area.output_4_GUI_th("Selected files:\n")
        for f in self.file_list:
            self.text_area.output_4_oth_th(f + "\n")

    def callback_load_files_button(self):
        if self.send_is_running.get():
            MyMsgBox(self, "Cannot load BITMAP files due to running file transmission to ITS board.", title="Error",
                     block=True)
            return
        help_text = "This list contains all BITMAP files given in file" + self.bitmap_list_file + ".\n\n" + \
                    "Select the set of files that should be sent to ITS board."
        try:
            with open(self.bitmap_list_file, 'r') as file:
                SelectBox(self, title="Load BITMAP file list from disk", button_text="select",
                          alt_range=[v[1].rstrip('\n') for v in enumerate(file.readlines()) if not v[1].isspace()],
                          selectmode='extended', help_text=help_text, action=lambda a1, a2: self.set_file_list(a2))
        except FileNotFoundError:
            MyMsgBox(self, "File " + self.bitmap_list_file + " listing all BITMAP files does not exist.",
                     title="Error", block=True)
            return

    def define_styles(self):
        bg_clr_enabled = 'grey80'
        bg_clr_disabled = 'grey95'
        print("theme list: " + str(self.style.theme_names()))
        self.style.theme_use('default')
        print("current theme: " + self.style.theme_use())
        self.style.configure('.', font=self.font)
        self.style.configure('MyButton.TButton', foreground='black')
        self.style.configure('FixButton.MyButton.TButton', width=10, foreground='black')
        self.style.configure('OneCharButton.MyButton.TButton', font=self.font + " bold", width=1)
        self.style.configure('MyLabel.TLabel', foreground='black', background=bg_clr_enabled)
        self.style.configure('MsgBox.TLabel', foreground='black', background='white')
        self.style.configure('MyFrame.TFrame', background=bg_clr_enabled)
        self.style.configure('MyEntry.TEntry', foreground='black', fieldbackground=bg_clr_enabled)
        self.style.configure('MyScale.Horizontal.TScale', background=bg_clr_enabled)
        self.style.configure('MyScale.Horizontal.TScale', troughcolor=bg_clr_enabled)
        self.style.map('MyFrame.TFrame', background=[('disabled', bg_clr_disabled), ('!disabled', bg_clr_enabled)])
        self.style.map('MyLabel.TLabel', background=[('disabled', bg_clr_disabled), ('!disabled', bg_clr_enabled)])
        self.style.map('MyEntry.TEntry', fieldbackground=[('disabled', bg_clr_disabled), ('!disabled', bg_clr_enabled)])
        self.style.map('MyScale.Horizontal.TScale',
                       background=[('disabled', bg_clr_disabled), ('!disabled', bg_clr_enabled)],
                       troughcolor=[('disabled', bg_clr_disabled), ('!disabled', bg_clr_enabled)])
