import threading


class ThreadSaveFlag:
    def __init__(self, val):
        self.__lock = threading.Lock()
        self.__flag = val

    def set(self, val):
        self.__lock.acquire()
        old_val = self.__flag
        self.__flag = val
        self.__lock.release()
        return old_val

    def get(self):
        self.__lock.acquire()
        val = self.__flag
        self.__lock.release()
        return val
