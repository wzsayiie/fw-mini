#include "mapp.h"

void CAppLaunch();
void CAppUpdate();

void CJsAppLaunch();
void CJsAppUpdate();

void _MAppLaunch() {
    
    CAppLaunch  ();
    CJsAppLaunch();
}

void _MAppUpdate() {
    
    CAppUpdate  ();
    CJsAppUpdate();
}
