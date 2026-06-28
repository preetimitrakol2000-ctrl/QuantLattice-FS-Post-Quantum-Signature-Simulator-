import ctypes
import os
import sys

class LatticeBridge:
    def __init__(self):
        if not os.path.exists("./liblattice.so") and not os.path.exists("./liblattice.dll"):
            if sys.platform.startswith("win"):
                os.system("gcc -shared -o liblattice.dll lattice_math.c")
                lib_path = "./liblattice.dll"
            else:
                os.system("gcc -shared -fPIC -o liblattice.so lattice_math.c")
                lib_path = "./liblattice.so"
        else:
            lib_path = "./liblattice.dll" if sys.platform.startswith("win") else "./liblattice.so"

        self.lib = ctypes.CDLL(lib_path)
        self.lib.init_lattice.restype = ctypes.c_void_p
        self.lib.compute_lattice_product.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_int)]
        self.lib.free_lattice.argtypes = [ctypes.c_void_p]
        
        self.package_ptr = self.lib.init_lattice()

    def generate_quantum_hash(self, secret_list: list):
        c_array = (ctypes.c_int * 4)(*secret_list)
        self.lib.compute_lattice_product(self.package_ptr, c_array)
        
        # Pull output data vector values out of structural layouts
        # Read the struct fields fields directly out of pointer tracking offsets
        result_ptr = ctypes.cast(self.package_ptr + (4*4*4) + (4*4), ctypes.POINTER(ctypes.c_int * 4))
        return [val for val in result_ptr.contents]

    def __del__(self):
        if hasattr(self, 'lib') and self.package_ptr:
            self.lib.free_lattice(self.package_ptr)
