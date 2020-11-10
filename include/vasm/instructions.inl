//name, code, args, execCode
#ifdef INST
#define SKIP

#ifndef INST_DOUBLE
#define INST_DOUBLE
#endif

#ifndef INST_REGISTER
#define INST_REGISTER
#endif

#ifndef INST_LABEL
#define INST_LABEL
#endif

#ifndef INST_COMPLEX
#define INST_COMPLEX
#endif

INST(sub,   0xA0, 0, SKIP)
INST(add,   0xA1, 0, SKIP)
INST(mul,   0xA2, 0, SKIP)
INST(div,   0xA3, 0, SKIP)
INST(subr,  0xA4, 0, SKIP)
INST(divr,  0xA5, 0, SKIP)
INST(inv,   0xA6, 0, SKIP)
INST(sqrt,  0xA7, 0, SKIP)
INST(cos,   0xAC, 0, SKIP)
INST(sin,   0xA8, 0, SKIP)
INST(round, 0xA9, 0, SKIP)
INST(pushd, 0xE0, 1, INST_DOUBLE)
INST(push,  0xE1, 1, INST_COMPLEX)
INST(pop,   0xE2, 0, SKIP)
INST(top,   0xE3, 1, INST_COMPLEX)
INST(in,    0xF0, 1, INST_REGISTER)
INST(out,   0xF1, 0, SKIP)
INST(dump,  0xD0, 0, SKIP)
INST(hlt,   0xC0, 0, SKIP)
INST(jmp,   0xC1, 1, INST_LABEL)
INST(jz,    0xC2, 1, INST_LABEL)
INST(jg,    0xC3, 1, INST_LABEL)
INST(jle,   0xC4, 1, INST_LABEL)
INST(call,  0xCF, 1, INST_LABEL)
INST(meow,  0xAA, 0, SKIP)
INST(ret,   0xCE, 0, SKIP)
INST(draw,  0xDA, 0, SKIP)

#undef INST
#undef SKIP
#undef INST_DOUBLE
#undef INST_REGISTER
#undef INST_LABEL
#undef INST_COMPLEX
#else
#error You need to define INST macro!
#endif