import tkinter as tk
import tkinter.ttk as ttk


class MyMsgBox(tk.Toplevel):
    """ Simple message box"""
    def __init__(self, master, text, **kwargs):
        # @param text displayed in the message box
        self.block = kwargs.pop('block', False)
        tk.Toplevel.__init__(self, master)
        self.configure(background="white")
        self.title(kwargs.pop('title', "Help"))
        self.geometry(kwargs.pop('geometry', "550x320"))
        orientation = {'left': (tk.NW, tk.LEFT), 'center': (tk.CENTER, tk.LEFT)}
        anchor = orientation[kwargs.pop('anchor', 'center')]
        self.label = ttk.Label(self, style='MsgBox.TLabel', text=text, anchor=anchor[0], justify=anchor[1])
        self.label.pack(fill="both", expand=True, padx=10, pady=10)
        self.button = ttk.Button(self, style='MyButton.TButton', text="o.k.", takefocus=False,
                                 command=lambda: self.destroy())
        self.button.pack(padx=10, pady=10)
        if self.block:
            master.wait_window(self)
