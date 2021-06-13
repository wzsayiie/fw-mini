#include <jni.h>
#include "mmain.h"

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MainActivity_startupMain(JNIEnv *, jobject)
{
    MMain();
}
