#define MEXPORT_CPP

#include "mexport.h"
#include "mdebug.h"

struct MFuncAdder {

    template<typename R, typename... A>
    MFuncAdder(const char *name, R (*func)(A... a)) {
    }
};

#include "MEXLIST.h"
