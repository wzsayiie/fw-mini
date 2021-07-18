#pragma once

#ifdef MEXPORT_CPP
    #define MFUNC_EXPORT(func) static FuncAdder _f_##func(#func, func);
#endif
