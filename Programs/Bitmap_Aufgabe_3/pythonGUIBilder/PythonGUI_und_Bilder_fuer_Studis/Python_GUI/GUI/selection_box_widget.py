import tkinter as tk
import tkinter.ttk as ttk
from GUI.help_button_widget import HelpButton
from GUI.my_msg_box_widget import MyMsgBox


class SelectBox:
    """ Simple message box to select an element out of a list"""
    def __init__(self, master, **kwargs):
        self.selectmode = kwargs.pop('selectmode', 'browse')
        self.alt_range = kwargs.pop('alt_range', ["NO ELEMENTS FOR SELECTION"])
        self.action = kwargs.pop('action', lambda w, t: print("Internal Error: No action defined"))
        self.my_box = tk.Toplevel(master)
        self.my_box.configure(background="white")
        self.my_box.title(kwargs.pop('title', " "))
        self.font = ttk.Style().lookup('.', 'font')
        help_button = HelpButton(self.my_box, master, help_text=kwargs.pop('help_text', "No help text"))
        help_button.grid(row=0, column=3, padx=10, pady=10)
        tk.Label(self.my_box, text='   ', bg='white').grid(row=0, column=0)  # define top margin in row 0
        tk.Label(self.my_box, text='   ', bg='white').grid(row=0, column=0)  # define left margin in column 0
        sbby = tk.Scrollbar(self.my_box)
        sbby.grid(row=1, column=2, sticky=tk.N + tk.S)
        sbbx = tk.Scrollbar(self.my_box)
        sbbx.grid(row=2, column=1, sticky=tk.W + tk.E)
        self.mylist = tk.Listbox(self.my_box, yscrollcommand=sbby.set,  xscrollcommand=sbbx.set,
                selectmode=self.selectmode, font=self.font, height=16, width=40, bd=3)
        self.mylist.grid(row=1, column=1, sticky=tk.N + tk.S + tk.E + tk.W)
        sbbx.config(command=self.mylist.xview, orient="horizontal")
        sbby.config(command=self.mylist.yview)
        for idx, elem in enumerate(self.alt_range):
            self.mylist.insert(idx, elem)
        self.button = ttk.Button(self.my_box, style='MyButton.TButton', text=kwargs.pop('button_text', "o.k."),
                                 takefocus=False, command=self.button_pressed)
        self.button.grid(row=3, column=0, columnspan=4, padx=20, pady=20)

    def button_pressed(self):
        if self.selectmode == 'browse':
            try:
                selected_elem = self.alt_range[self.mylist.curselection()[0]]
                self.action(self.my_box, selected_elem)
                self.my_box.destroy()
            except IndexError:
                MyMsgBox(self.my_box, "Please select one list element.", title="Error", block=True)
        if self.selectmode == 'extended':
            try:
                if 0 == len(self.mylist.curselection()):
                    raise IndexError
                self.action(self.my_box, [self.alt_range[i] for i in self.mylist.curselection()])
                self.my_box.destroy()
            except IndexError:
                MyMsgBox(self.my_box, "Please select at least one list element.", title="Error", block=True)
