#include <jni.h>
#include "mapp.h"
#include "mhostui.h"

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MainView_windowOnDraw(JNIEnv *, jobject)
{
    _MWindowOnDraw();
}

extern "C" JNIEXPORT jint JNICALL
Java_src_app_mini_MainView_windowTriangleCount(JNIEnv *, jobject)
{
    return _MWindowTriangleCount();
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_MainView_windowTriangleVertexX(JNIEnv *, jobject, jint index, jint vertexIndex)
{
    return _MWindowTriangleVertexX(index, vertexIndex);
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_MainView_windowTriangleVertexY(JNIEnv *, jobject, jint index, jint vertexIndex)
{
    return _MWindowTriangleVertexY(index, vertexIndex);
}

union AndroidColorPattern {
    struct {
        uint8_t blue ;
        uint8_t green;
        uint8_t red  ;
        uint8_t alpha;
    };
    int argb = 0;
};

extern "C" JNIEXPORT jint JNICALL
Java_src_app_mini_MainView_windowTriangleColor(JNIEnv *, jobject, jint index)
{
    MColorPattern src;
    src.rgba = _MWindowTriangleColor(index);

    AndroidColorPattern dst;
    dst.red   = src.red;
    dst.green = src.green;
    dst.blue  = src.blue;
    dst.alpha = src.alpha;

    return dst.argb;
}
