#include "mapp.h"
#include "mgraphics.h"
#include "mjnihelper.h"
#include "mwindow.h"

extern "C" JNIEXPORT JNICALL void
Java_src_app_mini_MAndrdMainActivity_setPixelScale(JNIEnv *, jclass, jfloat scale) {
    MSetPixelScale(scale);
}

extern "C" JNIEXPORT JNICALL jfloat
Java_src_app_mini_MAndrdMainActivity_appUpdateSeconds(JNIEnv *, jclass) {
    return MApp::UpdateEverySeconds;
}

extern "C" JNIEXPORT JNICALL void
Java_src_app_mini_MAndrdMainActivity_appLaunch(JNIEnv *, jclass) {
    MApp::instance()->launch();
}

extern "C" JNIEXPORT JNICALL void
Java_src_app_mini_MAndrdMainActivity_appUpdate(JNIEnv *, jclass) {
    MApp::instance()->update();
}

extern "C" JNIEXPORT JNICALL jfloat
Java_src_app_mini_MAndrdMainActivity_windowUpdateSeconds(JNIEnv *, jclass) {
    return MWindow::UpdateEverySeconds;
}

extern "C" JNIEXPORT JNICALL void
Java_src_app_mini_MAndrdMainActivity_windowResize(JNIEnv *, jclass, jfloat width, jfloat height) {
    MWindow::mainWindow()->resizePixel(width, height);
}

extern "C" JNIEXPORT JNICALL void
Java_src_app_mini_MAndrdMainActivity_windowLoad(JNIEnv *, jclass) {
    MWindow::mainWindow()->load();
}

extern "C" JNIEXPORT JNICALL void
Java_src_app_mini_MAndrdMainActivity_windowShow(JNIEnv *, jclass) {
    MWindow::mainWindow()->show();
}

extern "C" JNIEXPORT JNICALL void
Java_src_app_mini_MAndrdMainActivity_windowHide(JNIEnv *, jclass) {
    MWindow::mainWindow()->hide();
}

extern "C" JNIEXPORT JNICALL void
Java_src_app_mini_MAndrdMainActivity_windowTouchBegin(JNIEnv *, jclass, jfloat x, jfloat y) {
    auto evt = MTouch::makeBeginPixel(x, y, MTouchSource::Finger);
    MWindow::mainWindow()->touch(evt, nullptr);
}

extern "C" JNIEXPORT JNICALL void
Java_src_app_mini_MAndrdMainActivity_windowTouchMove(JNIEnv *, jclass, jfloat x, jfloat y) {
    auto evt = MTouch::makeMovePixel(x, y, MTouchSource::Finger);
    MWindow::mainWindow()->touch(evt, nullptr);
}

extern "C" JNIEXPORT JNICALL void
Java_src_app_mini_MAndrdMainActivity_windowTouchEnd(JNIEnv *, jclass, jfloat x, jfloat y) {
    auto evt = MTouch::makeEndPixel(x, y, MTouchSource::Finger);
    MWindow::mainWindow()->touch(evt, nullptr);
}

extern "C" JNIEXPORT JNICALL void
Java_src_app_mini_MAndrdMainActivity_windowKeyEnter(JNIEnv *, jclass) {
    auto evt = MKbKey::make(MKbKeyCode::Enter, 0);
    MWindow::mainWindow()->kbKey(evt);
}

extern "C" JNIEXPORT JNICALL void
Java_src_app_mini_MAndrdMainActivity_windowWriting(JNIEnv *, jclass, jstring jText) {
    auto evt = MWriting::make(m_cpp_string << jText);
    MWindow::mainWindow()->writing(evt);
}

extern "C" JNIEXPORT JNICALL jboolean
Java_src_app_mini_MAndrdMainActivity_checkWritingUpdated(JNIEnv *, jclass) {
    return MWindow::mainWindow()->checkWritingUpdated();
}

extern "C" JNIEXPORT JNICALL jboolean
Java_src_app_mini_MAndrdMainActivity_checkWritingEnabled(JNIEnv *, jclass) {
    return MWindow::mainWindow()->checkWritingEnabled();
}

extern "C" JNIEXPORT JNICALL jstring
Java_src_app_mini_MAndrdMainActivity_checkWritingRawText(JNIEnv *, jclass) {
    return m_local_jstring << MWindow::mainWindow()->checkWritingRawText();
}
