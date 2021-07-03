#define MEXPORT_CPP

#include "mexport.h"

struct FuncAdder {

    template<typename R, typename... A>
    FuncAdder(const char *name, R (*func)(A... a)) {
    }
};

#include "MEXLIST.h"
