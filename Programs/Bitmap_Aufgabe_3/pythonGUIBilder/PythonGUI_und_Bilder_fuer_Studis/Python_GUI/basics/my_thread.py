import threading
import ctypes


# An exception can be sent to this thread
# The function of the thread must handle
# the exception and can terminate due to this
# exception
class MyThread(threading.Thread):
    def __init__(self, target, args):
        threading.Thread.__init__(self, target=target, args=args)

    def __get_id(self):
        # returns thread id
        if hasattr(self, '_thread_id'):
            return self._thread_id
        for id, thread in threading._active.items():
            if thread is self:
                return id

    def raise_exception(self, ex=ctypes.py_object(SystemExit)):
        thread_id = self.__get_id()
        res = ctypes.pythonapi.PyThreadState_SetAsyncExc(thread_id, ex)
        if res > 1:
            ctypes.pythonapi.PyThreadState_SetAsyncExc(thread_id, 0)
            print('Exception raise failure')
