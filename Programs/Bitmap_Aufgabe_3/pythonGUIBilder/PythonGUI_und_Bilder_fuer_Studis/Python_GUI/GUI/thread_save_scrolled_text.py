import tkinter as tk
from tkinter.scrolledtext import ScrolledText
import queue
import threading


class MyScrolledText(ScrolledText):
    """ A thread, which runs in parallel to the GUI and writes to ScrolledText
        using insert, will be blocked in insert until the current GUI activity
        (like pressing a button) has been finished. This might lead to a deadlock.
        To avoid this kind of deadlocks a thread save queue and an additional
        thread will be used. This thread sends the GUI event <<UpdateLog>> such
        that the queue will be flushed.
        Hence, the parallel running thread does not block, even if the event sending
        thread blocks.
    """

    def __init__(self, master, **kwargs):
        ScrolledText.__init__(self)
        self.__queue = queue.Queue()
        self.bind("<<UpdateLog>>", lambda x: self.__flush_queue())

    def __flush_queue(self):
        while not self.__queue.empty():
            self.output_4_GUI_th(self.__queue.get())

    def output_4_GUI_th(self, text):
        """ This function should be used by methods implementing an event of the GUI"""
        self.configure(state='normal')
        self.insert(tk.INSERT, text)
        self.configure(state='disabled')
        self.yview(tk.END)

    def output_4_oth_th(self, text):
        """ This function should be used by threads running in parallel to GUI"""
        self.__queue.put(text)
        t = threading.Thread(target=lambda: self.event_generate("<<UpdateLog>>"))
        t.start()
