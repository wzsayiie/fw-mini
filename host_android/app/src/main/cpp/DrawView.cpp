#include "AndroidApi.h"
#include "mhostui.h"

union AndroidColorPattern
{
    struct
    {
        uint8_t blue ;
        uint8_t green;
        uint8_t red  ;
        uint8_t alpha;
    };
    int argb = 0;
};

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_DrawView_windowOnDraw(JNIEnv *, jobject)
{
    _MWindowOnDraw();
}

extern "C" JNIEXPORT jint JNICALL
Java_src_app_mini_DrawView_windowGraphCount(JNIEnv *, jobject)
{
    return _MWindowGraphCount();
}

extern "C" JNIEXPORT jint JNICALL
Java_src_app_mini_DrawView_windowGraphType(JNIEnv *, jobject, jint index)
{
    return _MWindowGraphType(index);
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_DrawView_windowTriangleGraphX0(JNIEnv *, jobject, jint index)
{
    return _MWindowTriangleGraphX0(index);
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_DrawView_windowTriangleGraphY0(JNIEnv *, jobject, jint index)
{
    return _MWindowTriangleGraphY0(index);
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_DrawView_windowTriangleGraphX1(JNIEnv *, jobject, jint index)
{
    return _MWindowTriangleGraphX1(index);
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_DrawView_windowTriangleGraphY1(JNIEnv *, jobject, jint index)
{
    return _MWindowTriangleGraphY1(index);
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_DrawView_windowTriangleGraphX2(JNIEnv *, jobject, jint index)
{
    return _MWindowTriangleGraphX2(index);
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_DrawView_windowTriangleGraphY2(JNIEnv *, jobject, jint index)
{
    return _MWindowTriangleGraphY2(index);
}

extern "C" JNIEXPORT jint JNICALL
Java_src_app_mini_DrawView_windowTriangleGraphColor(JNIEnv *, jobject, jint index)
{
    MColorPattern src;
    src.rgba = _MWindowTriangleGraphColor(index);

    AndroidColorPattern dst;
    dst.red   = src.red;
    dst.green = src.green;
    dst.blue  = src.blue;
    dst.alpha = src.alpha;

    return dst.argb;
}

extern "C" JNIEXPORT jobject JNICALL
Java_src_app_mini_DrawView_windowImageGraphObject(JNIEnv *, jobject, jint index)
{
    auto image = (MAndroidImage *)_MWindowImageGraphObject(index);
    return image->nativeImage();
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_DrawView_windowImageGraphX(JNIEnv *, jobject, jint index)
{
    return _MWindowImageGraphX(index);
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_DrawView_windowImageGraphY(JNIEnv *, jobject, jint index)
{
    return _MWindowImageGraphY(index);
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_DrawView_windowImageGraphWidth(JNIEnv *, jobject, jint index)
{
    return _MWindowImageGraphWidth(index);
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_DrawView_windowImageGraphHeight(JNIEnv *, jobject, jint index)
{
    return _MWindowImageGraphHeight(index);
}

extern "C" JNIEXPORT jstring JNICALL
Java_src_app_mini_DrawView_windowLabelGraphString(JNIEnv *env, jobject, jint index)
{
    MString *string = _MWindowLabelGraphString(index);

    auto chars = (const jchar *)MStringU16Chars(string);
    auto size  = (jsize)MStringU16Size(string);

    return env->NewString(chars, size);
}

extern "C" JNIEXPORT jint JNICALL
Java_src_app_mini_DrawView_windowLabelGraphColor(JNIEnv *, jobject, jint index)
{
    MColorPattern src;
    src.rgba = _MWindowLabelGraphColor(index);

    AndroidColorPattern dst;
    dst.red   = src.red;
    dst.green = src.green;
    dst.blue  = src.blue;
    dst.alpha = src.alpha;

    return dst.argb;
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_DrawView_windowLabelGraphFontSize(JNIEnv *, jobject, jint index)
{
    return _MWindowLabelGraphFontSize(index);
}

extern "C" JNIEXPORT jint JNICALL
Java_src_app_mini_DrawView_windowLabelGraphHAlign(JNIEnv *, jobject, jint index)
{
    return _MWindowLabelGraphHAlign(index);
}

extern "C" JNIEXPORT jint JNICALL
Java_src_app_mini_DrawView_windowLabelGraphVAlign(JNIEnv *, jobject, jint index)
{
    return _MWindowLabelGraphVAlign(index);
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_DrawView_windowLabelGraphX(JNIEnv *, jobject, jint index)
{
    return _MWindowLabelGraphX(index);
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_DrawView_windowLabelGraphY(JNIEnv *, jobject, jint index)
{
    return _MWindowLabelGraphY(index);
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_DrawView_windowLabelGraphWidth(JNIEnv *, jobject, jint index)
{
    return _MWindowLabelGraphWidth(index);
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_DrawView_windowLabelGraphHeight(JNIEnv *, jobject, jint index)
{
    return _MWindowLabelGraphHeight(index);
}
