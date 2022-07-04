#include "mapp.h"
#include "mgraphics.h"
#include "mjnihelper.h"
#include "mwindow.h"

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MAndrdMainActivity_setPixelScale(JNIEnv *, jclass, jfloat scale) {
    MSetPixelScale(scale);
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_MAndrdMainActivity_appUpdateSeconds(JNIEnv *, jclass) {
    return MApp::UpdateEverySeconds;
}

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MAndrdMainActivity_appLaunch(JNIEnv *, jclass) {
    MApp::instance()->launch();
}

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MAndrdMainActivity_appUpdate(JNIEnv *, jclass) {
    MApp::instance()->update();
}

extern "C" JNIEXPORT jfloat JNICALL
Java_src_app_mini_MAndrdMainActivity_windowUpdateSeconds(JNIEnv *, jclass) {
    return MWindow::UpdateEverySeconds;
}

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MAndrdMainActivity_windowResize(JNIEnv *, jclass, jfloat width, jfloat height) {
    MWindow::mainWindow()->resizePixel(width, height);
}

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MAndrdMainActivity_windowLoad(JNIEnv *, jclass) {
    MWindow::mainWindow()->load();
}

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MAndrdMainActivity_windowShow(JNIEnv *, jclass) {
    MWindow::mainWindow()->show();
}

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MAndrdMainActivity_windowHide(JNIEnv *, jclass) {
    MWindow::mainWindow()->hide();
}

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MAndrdMainActivity_windowTouchBegin(JNIEnv *, jclass, jfloat x, jfloat y) {
    MWindow::mainWindow()->touchBeginPixel(x, y);
}

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MAndrdMainActivity_windowTouchMove(JNIEnv *, jclass, jfloat x, jfloat y) {
    MWindow::mainWindow()->touchMovePixel(x, y);
}

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MAndrdMainActivity_windowTouchEnd(JNIEnv *, jclass, jfloat x, jfloat y) {
    MWindow::mainWindow()->touchEndPixel(x, y);
}

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MAndrdMainActivity_windowWrite(JNIEnv *, jclass, jstring jText, jboolean enter) {
    MWindow::mainWindow()->write(mini_cpp_string jText, enter);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_src_app_mini_MAndrdMainActivity_checkWritingUpdated(JNIEnv *, jclass) {
    return MWindow::mainWindow()->checkWritingUpdated();
}

extern "C" JNIEXPORT jboolean JNICALL
Java_src_app_mini_MAndrdMainActivity_checkWritingEnabled(JNIEnv *, jclass) {
    return MWindow::mainWindow()->checkWritingEnabled();
}

extern "C" JNIEXPORT jstring JNICALL
Java_src_app_mini_MAndrdMainActivity_checkWritingRawText(JNIEnv *, jclass) {
    return mini_local_jstring MWindow::mainWindow()->checkWritingRawText();
}
