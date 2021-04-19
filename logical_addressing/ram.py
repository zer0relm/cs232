RAM_SIZE = 1024


class RAM:
    def __init__(self, size=RAM_SIZE):
        self._minAddr = 0
        self._maxAddr = RAM_SIZE - 1
        self._memory = []   # a list of values.  Could be #s or instructions.
        for i in range(size):
            self._memory.append(0)

    def __getitem__(self, addr):
        return self._memory[addr]

    def __setitem__(self, addr, val):
        self._memory[addr] = val

    def is_legal_addr(self, addr):
        return self._minAddr <= addr <= self._maxAddr

class MMU:
    def __init__(self, RAM):
        self._ram = RAM
        self._reloc_register = 0
        self._limit_register = 0
    
    def set_reloc_register(self, value):
        self._reloc_register = value

    def set_limit_register(self, value):
        self._limit_register = value

    def getvalue(self, addr):
        return self._ram.__getitem__(addr)
        
    def setvalue(self, addr, value):
        self._ram.__setitem__(addr, value)

    def check_logical_addr(self, addr):
        if (addr < (self._reloc_register + self._limit_register)):
            return True
        else:
            print("Bad Address: %i: too high", format(addr))
            return False
    