#pragma once

#ifdef MEXPORT_CPP
    #define MEXPORT(func) static FuncAdder _f_##func(#func, func);
#else
    #define MEXPORT(func)
#endif
