from ctypes import cdll, c_char_p

class cpp:
    def __init__(self):
        #dynamic_library_file = r"C:\Users\wolu\robot-framework-cpp\cpp\cmake-build-debug\robot-framework-cpp.dll"
        dynamic_library_file = "/Users/luki/Development/robot-framework-cpp/cpp/build/librobot-framework-cpp.dylib"
        self.cpplib = cdll.LoadLibrary(dynamic_library_file)

    def get_keyword_names(self):
        get_keyword_names = self.cpplib.get_keyword_names
        get_keyword_names.restype = c_char_p
        print(get_keyword_names())
        return [get_keyword_names()]

    def _call_cpp_function(self, name):
        print(f"Hello {name}")


    def run_keyword(self, name, args, kwargs):
        print("Running keyword '%s' with positional arguments %s and named arguments %s."
              % (name, args, kwargs))
