#include <jni.h>
#include "mhostui.h"

union AndroidColorPattern {
    struct {
        uint8_t blue ;
        uint8_t green;
        uint8_t red  ;
        uint8_t alpha;
    };
    int argb = 0;
};

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

extern "C" JNIEXPORT jint JNICALL
Java_src_app_mini_MainView_windowImageCount(JNIEnv *, jobject)
{
    return _MWindowImageCount();
}

extern "C" JNIEXPORT jint JNICALL
Java_src_app_mini_MainView_windowImageObject(JNIEnv *, jobject, jint index)
{
    MImage *image = _MWindowImageObject(index);
    return MImageManagedId(image);
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_MainView_windowImageX(JNIEnv *, jobject, jint index)
{
    return _MWindowImageX(index);
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_MainView_windowImageY(JNIEnv *, jobject, jint index)
{
    return _MWindowImageY(index);
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_MainView_windowImageWidth(JNIEnv *, jobject, jint index)
{
    return _MWindowImageWidth(index);
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_MainView_windowImageHeight(JNIEnv *, jobject, jint index)
{
    return _MWindowImageHeight(index);
}

extern "C" JNIEXPORT jint JNICALL
Java_src_app_mini_MainView_windowLabelCount(JNIEnv *, jobject)
{
    return _MWindowLabelCount();
}

extern "C" JNIEXPORT jstring JNICALL
Java_src_app_mini_MainView_windowLabelString(JNIEnv *env, jobject, jint index)
{
    MString *string = _MWindowLabelString(index);

    auto chars = (const jchar *)MStringU16Chars(string);
    auto size  = (jsize)MStringU16Size(string);

    return env->NewString(chars, size);
}

extern "C" JNIEXPORT jint JNICALL
Java_src_app_mini_MainView_windowLabelColor(JNIEnv *, jobject, jint index)
{
    MColorPattern src;
    src.rgba = _MWindowLabelColor(index);

    AndroidColorPattern dst;
    dst.red   = src.red;
    dst.green = src.green;
    dst.blue  = src.blue;
    dst.alpha = src.alpha;

    return dst.argb;
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_MainView_windowLabelFontSize(JNIEnv *, jobject, jint index)
{
    return _MWindowLabelFontSize(index);
}

extern "C" JNIEXPORT jint JNICALL
Java_src_app_mini_MainView_windowLabelHAlign(JNIEnv *, jobject, jint index)
{
    return _MWindowLabelHAlign(index);
}

extern "C" JNIEXPORT jint JNICALL
Java_src_app_mini_MainView_windowLabelVAlign(JNIEnv *, jobject, jint index)
{
    return _MWindowLabelVAlign(index);
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_MainView_windowLabelX(JNIEnv *, jobject, jint index)
{
    return _MWindowLabelX(index);
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_MainView_windowLabelY(JNIEnv *, jobject, jint index)
{
    return _MWindowLabelY(index);
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_MainView_windowLabelWidth(JNIEnv *, jobject, jint index)
{
    return _MWindowLabelWidth(index);
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_MainView_windowLabelHeight(JNIEnv *, jobject, jint index)
{
    return _MWindowLabelHeight(index);
}
