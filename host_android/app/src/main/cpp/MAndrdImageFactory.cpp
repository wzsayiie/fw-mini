#include "MAndrdImageFactory.h"

const char *MAndrdImageFactoryJClassPath = "src/app/mini/MAndrdImageFactory";

minikit_class(MAndrdImageFactory, MImageFactory) {
public:
    MImage::ptr onDecodeFFData(const MVector<uint8_t>::ptr &ffData) override;
    MImage::ptr onDecodeBitmap(const MVector<uint8_t>::ptr &bitDat, int width, int height) override;

    MVector<uint8_t>::ptr onEncodeFFData(const MImage::ptr &image, MImageFileFormat format) override;
    MVector<uint8_t>::ptr onEncodeBitmap(const MImage::ptr &image) override;

    MSize::ptr onGetPixelSize(const MImage::ptr &image) override;
};

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MAndrdImageFactory_install(JNIEnv *, jclass) {
    auto obj = MAndrdImageFactory::create();
    MImageFactory::setInstance(obj);
}

MImage::ptr MAndrdImageFactory::onDecodeFFData(const MVector<uint8_t>::ptr &ffData) {
    static jclass    cls = MJNINewGlobalJClass(MAndrdImageFactoryJClassPath);
    static jmethodID mtd = MJNIGetEnv()->GetStaticMethodID(cls, __func__, "([B)Landroid/graphics/Bitmap;");

    jbyteArray jFFData = MJNINewLocalJBytes(ffData);
    jobject    jBitmap = MJNIGetEnv()->CallStaticObjectMethod(cls, mtd, jFFData);

    if (jBitmap) {
        auto image = MAndrdImage::create();
        image->mBitmap = jBitmap;
        return image;
    }
    return nullptr;
}

MImage::ptr MAndrdImageFactory::onDecodeBitmap(const MVector<uint8_t>::ptr &bitDat, int width, int height) {
    static jclass    cls = MJNINewGlobalJClass(MAndrdImageFactoryJClassPath);
    static jmethodID mtd = MJNIGetEnv()->GetStaticMethodID(cls, __func__, "([BII)Landroid/graphics/Bitmap;");

    jbyteArray jBitDat = MJNINewLocalJBytes(bitDat);
    jobject    jBitmap = MJNIGetEnv()->CallStaticObjectMethod(cls, mtd, jBitDat, width, height);

    if (jBitmap) {
        auto image = MAndrdImage::create();
        image->mBitmap = jBitmap;
        return image;
    }
    return nullptr;
}

MVector<uint8_t>::ptr MAndrdImageFactory::onEncodeFFData(const MImage::ptr &image, MImageFileFormat format) {
    static jclass    cls = MJNINewGlobalJClass(MAndrdImageFactoryJClassPath);
    static jmethodID mtd = MJNIGetEnv()->GetStaticMethodID(cls, __func__, "(Landroid/graphics/Bitmap;C)[B");

    jchar jFormat = 0;
    switch (format) {
        case MImageFileFormat::JPEG: jFormat = 'J'; break;
        case MImageFileFormat::PNG : jFormat = 'P'; break;
    }

    jobject jBitmap = ((MAndrdImage *)image.get())->mBitmap;
    auto    jFFData = (jbyteArray)MJNIGetEnv()->CallStaticObjectMethod(cls, mtd, jBitmap, jFormat);

    return MJNIGetBytes(jFFData);
}

MVector<uint8_t>::ptr MAndrdImageFactory::onEncodeBitmap(const MImage::ptr &image) {
    static jclass    cls = MJNINewGlobalJClass(MAndrdImageFactoryJClassPath);
    static jmethodID mtd = MJNIGetEnv()->GetStaticMethodID(cls, __func__, "(Landroid/graphics/Bitmap;)[B");

    jobject jBitmap = ((MAndrdImage *)image.get())->mBitmap;
    auto    jBitDat = (jbyteArray)MJNIGetEnv()->CallStaticObjectMethod(cls, mtd, jBitmap);

    return MJNIGetBytes(jBitDat);
}

MSize::ptr MAndrdImageFactory::onGetPixelSize(const MImage::ptr &image) {
    static jclass    cls = MJNINewGlobalJClass(MAndrdImageFactoryJClassPath);
    static jmethodID wfn = MJNIGetEnv()->GetStaticMethodID(cls, "onGetPixelX", "(Landroid/graphics/Bitmap;)F");
    static jmethodID hfn = MJNIGetEnv()->GetStaticMethodID(cls, "onGetPixelY", "(Landroid/graphics/Bitmap;)F");

    jobject jBitmap = ((MAndrdImage *)image.get())->mBitmap;
    float   width   = MJNIGetEnv()->CallStaticFloatMethod(cls, wfn, jBitmap);
    float   height  = MJNIGetEnv()->CallStaticFloatMethod(cls, hfn, jBitmap);

    return MSize::from(width, height);
}
