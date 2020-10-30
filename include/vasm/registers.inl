#ifdef REG
REG(rax, 0)
REG(rbx, 1)
REG(rcx, 2)
REG(rdx, 3)
REG(rex, 4)
REG(rx,  5)
REG(ry,  6)
REG(rcl, 7)
REG(rg,  8)
REG(rad, 9)
REG(rcy, 10)
#undef REG
#else
#error You need to define REG macro!
#endif