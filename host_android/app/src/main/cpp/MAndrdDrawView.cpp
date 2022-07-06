#include "MAndrdImageFactory.h"
#include "mcontext.h"
#include "mwindow.h"

static MGraph        *sGraph   = nullptr;
static MClipGraph    *sClip    = nullptr;
static MPolygonGraph *sPolygon = nullptr;
static MImageGraph   *sImage   = nullptr;
static MTextGraph    *sText    = nullptr;

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MAndrdDrawView_drawGraphs(JNIEnv *, jclass) {
    MWindow::mainWindow()->draw();
}

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MAndrdDrawView_clearGraphs(JNIEnv *, jclass) {
    MContextReset();
}

extern "C" JNIEXPORT jint JNICALL
Java_src_app_mini_MAndrdDrawView_graphCount(JNIEnv *, jclass) {
    return (jint)MContextGetGraphs()->size();
}

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MAndrdDrawView_selectGraph(JNIEnv *, jclass, jint index) {
    sGraph = MContextGetGraphs()->at(index).get();

    sClip    = (MClipGraph    *)sGraph;
    sPolygon = (MPolygonGraph *)sGraph;
    sImage   = (MImageGraph   *)sGraph;
    sText    = (MTextGraph    *)sGraph;
}

extern "C" JNIEXPORT jchar JNICALL
Java_src_app_mini_MAndrdDrawView_graphType(JNIEnv *, jclass) {
    switch (sGraph->type()) {
        case MGraphType::Clip   : return 'C';
        case MGraphType::Polygon: return 'P';
        case MGraphType::Image  : return 'I';
        case MGraphType::Text   : return 'T';

        default: return 0;
    }
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_MAndrdDrawView_clipX(JNIEnv *, jclass) {
    return sClip->pixelX();
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_MAndrdDrawView_clipY(JNIEnv *, jclass) {
    return sClip->pixelY();
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_MAndrdDrawView_clipWidth(JNIEnv *, jclass) {
    return sClip->pixelW();
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_MAndrdDrawView_clipHeight(JNIEnv *, jclass) {
    return sClip->pixelH();
}

extern "C" JNIEXPORT jint JNICALL
Java_src_app_mini_MAndrdDrawView_polygonPoints(JNIEnv *, jclass) {
    return sPolygon->points();
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_MAndrdDrawView_polygonPointX(JNIEnv *, jclass, jint index) {
    return sPolygon->pixelX(index);
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_MAndrdDrawView_polygonPointY(JNIEnv *, jclass, jint index) {
    return sPolygon->pixelY(index);
}

extern "C" JNIEXPORT jint JNICALL
Java_src_app_mini_MAndrdDrawView_polygonARGB(JNIEnv *, jclass) {
    //IMPORTANT: color byte order transform.
    int color = sPolygon->rgba();
    MColorTransform((MColorRGBA *)&color, (MEarlyARGB *)&color, 1);
    return color;
}

extern "C" JNIEXPORT jobject JNICALL
Java_src_app_mini_MAndrdDrawView_imageObject(JNIEnv *, jclass) {
    auto image = (MAndrdImage *)sImage->image().get();
    return image->mBitmap.get();
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_MAndrdDrawView_imageX(JNIEnv *, jclass) {
    return sImage->pixelX();
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_MAndrdDrawView_imageY(JNIEnv *, jclass) {
    return sImage->pixelY();
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_MAndrdDrawView_imageWidth(JNIEnv *, jclass) {
    return sImage->pixelW();
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_MAndrdDrawView_imageHeight(JNIEnv *, jclass) {
    return sImage->pixelH();
}

extern "C" JNIEXPORT jstring JNICALL
Java_src_app_mini_MAndrdDrawView_textString(JNIEnv *, jclass) {
    return m_local_jstring sText->text();
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_MAndrdDrawView_textFontSize(JNIEnv *, jclass) {
    return sText->pixelFontSize();
}

extern "C" JNIEXPORT jint JNICALL
Java_src_app_mini_MAndrdDrawView_textARGB(JNIEnv *, jclass) {
    //IMPORTANT: color byte order transform.
    int color = sText->rgba();
    MColorTransform((MColorRGBA *)&color, (MEarlyARGB *)&color, 1);
    return color;
}

extern "C" JNIEXPORT jchar JNICALL
Java_src_app_mini_MAndrdDrawView_textHAlign(JNIEnv *, jclass) {
    switch (sText->hAlign()) {
        case MHAlign::Left  : return 'L';
        case MHAlign::Center: return 'C';
        case MHAlign::Right : return 'R';

        default: return 0;
    }
}

extern "C" JNIEXPORT jchar JNICALL
Java_src_app_mini_MAndrdDrawView_textVAlign(JNIEnv *, jclass) {
    switch (sText->vAlign()) {
        case MVAlign::Top   : return 'T';
        case MVAlign::Middle: return 'M';
        case MVAlign::Bottom: return 'B';

        default: return 0;
    }
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_MAndrdDrawView_textX(JNIEnv *, jclass) {
    return sText->pixelX();
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_MAndrdDrawView_textY(JNIEnv *, jclass) {
    return sText->pixelY();
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_MAndrdDrawView_textWidth(JNIEnv *, jclass) {
    return sText->pixelW();
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_MAndrdDrawView_textHeight(JNIEnv *, jclass) {
    return sText->pixelH();
}
