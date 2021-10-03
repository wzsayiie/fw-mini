#include "jnihelper.h"
#include "mhostloop.h"
#include "mhostui.h"

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_MainActivity_constAppUpdateInterval(JNIEnv *, jobject)
{
    return _MAppUpdateInterval;
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_MainActivity_constWindowDrawInterval(JNIEnv *, jobject)
{
    return _MWindowDrawInterval;
}

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MainActivity_appLaunch(JNIEnv *, jobject)
{
    _MAppLaunch();
}

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MainActivity_appUpdate(JNIEnv *, jobject)
{
    _MAppUpdate();
}

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MainActivity_windowSetPixelDensity(JNIEnv *, jobject, jfloat density)
{
    _MWindowSetPixelDensity(density);
}

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MainActivity_windowOnLoad(JNIEnv *, jobject)
{
    _MWindowOnLoad();
}

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MainActivity_windowOnShow(JNIEnv *, jobject)
{
    _MWindowOnShow();
}

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MainActivity_windowOnHide(JNIEnv *, jobject)
{
    _MWindowOnHide();
}

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MainActivity_windowOnResize(JNIEnv *, jobject, jfloat width, jfloat height)
{
    _MWindowOnResize(width, height);
}

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MainActivity_windowOnTouchBegin(JNIEnv *, jobject, jfloat x, jfloat y)
{
    _MWindowOnTouchBegin(x, y);
}

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MainActivity_windowOnTouchMove(JNIEnv *, jobject, jfloat x, jfloat y)
{
    _MWindowOnTouchMove(x, y);
}

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MainActivity_windowOnTouchEnd(JNIEnv *, jobject, jfloat x, jfloat y)
{
    _MWindowOnTouchEnd(x, y);
}

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MainActivity_windowOnTextBox(JNIEnv *env, jobject, jstring string, jboolean enter)
{
    MStringRef text = m_auto_release MStringCopyJObject(env, string);
    _MWindowOnTextBox(text.get(), enter);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_src_app_mini_MainActivity_windowTextBoxUpdated(JNIEnv *, jobject)
{
    return _MWindowTextBoxUpdated();
}

extern "C" JNIEXPORT jboolean JNICALL
Java_src_app_mini_MainActivity_windowTextBoxEnabled(JNIEnv *, jobject)
{
    return _MWindowTextBoxEnabled();
}

extern "C" JNIEXPORT jstring JNICALL
Java_src_app_mini_MainActivity_windowTextBoxRawString(JNIEnv *env, jobject)
{
    MString *string = _MWindowTextBoxRawString();
    return JNewString(env, string);
}

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MainActivity_windowSetTextBoxUpdated(JNIEnv *, jobject, jboolean updated)
{
    MWindowSetTextBoxUpdated(updated);
}
