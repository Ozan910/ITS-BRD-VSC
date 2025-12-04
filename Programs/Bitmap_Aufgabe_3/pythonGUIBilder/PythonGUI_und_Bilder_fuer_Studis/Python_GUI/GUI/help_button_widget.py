import tkinter.ttk as ttk
from GUI.my_msg_box_widget import MyMsgBox


class HelpButton(ttk.Button):
    def __init__(self, parent, top_widget, **kwargs):
        # @param help_text Text that will be displayed in help message box
        self.help_text = kwargs.pop('help_text', "no help_text")
        self.parent, self.top_widget = parent, top_widget
        ttk.Button.__init__(self, self.parent, style='OneCharButton.MyButton.TButton', takefocus=False,
                            command=lambda: MyMsgBox(self.top_widget, self.help_text), text='?', width=1)
