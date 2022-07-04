#include "MAndrdImageFactory.h"

minikit_class(MAndrdImageFactory, MImageFactory) {
public:
    MImage::ptr onDecodeFFData(const MVector<uint8_t>::ptr &ffData) override;
    MImage::ptr onDecodeBitmap(const MVector<uint8_t>::ptr &bitDat, int width, int height) override;

    MVector<uint8_t>::ptr onEncodeFFData(const MImage::ptr &image, MImageFileFormat format) override;
    MVector<uint8_t>::ptr onEncodeBitmap(const MImage::ptr &image) override;

    MSize::ptr onGetPixelSize(const MImage::ptr &image) override;
};

static jclass GetGlobalJClass() {
    static jclass global = mini_global_jclass "src/app/mini/MAndrdImageFactory";
    return global;
}
#define cls GetGlobalJClass()

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MAndrdImageFactory_install(JNIEnv *, jclass) {
    auto obj = MAndrdImageFactory::create();
    MImageFactory::setInstance(obj);
}

MImage::ptr MAndrdImageFactory::onDecodeFFData(const MVector<uint8_t>::ptr &ffData) {
    static jmethodID method = mini_jni_env->GetStaticMethodID(cls, __func__, "([B)Landroid/graphics/Bitmap;");

    jbyteArray jFFData = mini_local_jbytes ffData;
    jobject    jBitmap = mini_jni_env->CallStaticObjectMethod(cls, method, jFFData);

    if (jBitmap) {
        auto image = MAndrdImage::create();
        image->mBitmap = mini_global_jobject jBitmap;
        return image;
    }
    return nullptr;
}

MImage::ptr MAndrdImageFactory::onDecodeBitmap(const MVector<uint8_t>::ptr &bitDat, int width, int height) {
    static jmethodID method = mini_jni_env->GetStaticMethodID(cls, __func__, "([BII)Landroid/graphics/Bitmap;");

    jbyteArray jBitDat = mini_local_jbytes bitDat;
    jobject    jBitmap = mini_jni_env->CallStaticObjectMethod(cls, method, jBitDat, width, height);

    if (jBitmap) {
        auto image = MAndrdImage::create();
        image->mBitmap = mini_global_jobject jBitmap;
        return image;
    }
    return nullptr;
}

MVector<uint8_t>::ptr MAndrdImageFactory::onEncodeFFData(const MImage::ptr &image, MImageFileFormat format) {
    static jmethodID method = mini_jni_env->GetStaticMethodID(cls, __func__, "(Landroid/graphics/Bitmap;C)[B");

    jchar jFormat = 0;
    switch (format) {
        case MImageFileFormat::JPEG: jFormat = 'J'; break;
        case MImageFileFormat::PNG : jFormat = 'P'; break;
    }

    jobject jBitmap = ((MAndrdImage *)image.get())->mBitmap.get();
    auto    jFFData = (jbyteArray)mini_jni_env->CallStaticObjectMethod(cls, method, jBitmap, jFormat);

    return mini_cpp_bytes jFFData;
}

MVector<uint8_t>::ptr MAndrdImageFactory::onEncodeBitmap(const MImage::ptr &image) {
    static jmethodID method = mini_jni_env->GetStaticMethodID(cls, __func__, "(Landroid/graphics/Bitmap;)[B");

    jobject jBitmap = ((MAndrdImage *)image.get())->mBitmap.get();
    auto    jBitDat = (jbyteArray)mini_jni_env->CallStaticObjectMethod(cls, method, jBitmap);

    return mini_cpp_bytes jBitDat;
}

MSize::ptr MAndrdImageFactory::onGetPixelSize(const MImage::ptr &image) {
    static jmethodID wFunc = mini_jni_env->GetStaticMethodID(cls, "onGetPixelX", "(Landroid/graphics/Bitmap;)F");
    static jmethodID hFunc = mini_jni_env->GetStaticMethodID(cls, "onGetPixelY", "(Landroid/graphics/Bitmap;)F");

    jobject jBitmap = ((MAndrdImage *)image.get())->mBitmap.get();
    float   width   = mini_jni_env->CallStaticFloatMethod(cls, wFunc, jBitmap);
    float   height  = mini_jni_env->CallStaticFloatMethod(cls, hFunc, jBitmap);

    return MSize::from(width, height);
}
