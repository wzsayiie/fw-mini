#include "mapp.h"

void CAppLaunch();
void CAppUpdate();

void JsAppLaunch();
void JsAppUpdate();

void _MAppLaunch() {
    
    CAppLaunch ();
    JsAppLaunch();
}

void _MAppUpdate() {
    
    CAppUpdate ();
    JsAppUpdate();
}
