#include "MAndrdImageFactory.h"
#include "mgraphics.h"
#include "mjnihelper.h"

static jclass C() {
    static std::shared_ptr<_jclass> cls = m_global_jclass << "src/app/mini/MAndrdImageFactory";
    return cls.get();
}

extern "C" JNIEXPORT JNICALL void
Java_src_app_mini_MAndrdImageFactory_install(JNIEnv *, jclass) {
    auto obj = MAndrdImageFactory::create();
    MImageFactory::setInstance(obj);
}

MImage::ptr MAndrdImageFactory::onDecodeFFData(const MData::ptr &ffData) {
    static jmethodID method = m_jenv->GetStaticMethodID(C(), __func__, "([B)Landroid/graphics/Bitmap;");

    jbyteArray jFFData = m_local_jbytes << ffData;
    jobject    jBitmap = m_jenv->CallStaticObjectMethod(C(), method, jFFData);

    if (jBitmap) {
        auto image = MAndrdImage::create();
        image->mBitmap = m_global_jobject << jBitmap;
        return image;
    }
    return nullptr;
}

MImage::ptr MAndrdImageFactory::onDecodeBitmap(const MData::ptr &bitDat, int width, int height) {
    static jmethodID method = m_jenv->GetStaticMethodID(C(), __func__, "([III)Landroid/graphics/Bitmap;");

    //IMPORTANT: color byte order transform.
    std::vector<int> pixels; {
        pixels.resize(width * height);

        auto src = (MColorRGBA *)bitDat->bytes();
        auto dst = (MEarlyARGB *)pixels.data();
        MColorTransform(src, dst, width * height);
    }

    jintArray jBitDat = m_local_jints << pixels;
    jobject   jBitmap = m_jenv->CallStaticObjectMethod(C(), method, jBitDat, width, height);

    if (jBitmap) {
        auto image = MAndrdImage::create();
        image->mBitmap = m_global_jobject << jBitmap;
        return image;
    }
    return nullptr;
}

MData::ptr MAndrdImageFactory::onEncodeFFData(const MImage::ptr &image, MImageFileFormat format) {
    static jmethodID method = m_jenv->GetStaticMethodID(C(), __func__, "(Landroid/graphics/Bitmap;C)[B");

    jchar jFormat = 0;
    switch (format) {
        case MImageFileFormat::JPEG: jFormat = 'J'; break;
        case MImageFileFormat::PNG : jFormat = 'P'; break;
    }

    jobject jBitmap = ((MAndrdImage *)image.get())->mBitmap.get();
    jobject jFFData = m_jenv->CallStaticObjectMethod(C(), method, jBitmap, jFormat);

    return m_cpp_bytes << jFFData;
}

MData::ptr MAndrdImageFactory::onEncodeBitmap(const MImage::ptr &image) {
    static jmethodID method = m_jenv->GetStaticMethodID(C(), __func__, "(Landroid/graphics/Bitmap;)[I");

    jobject jBitmap = ((MAndrdImage *)image.get())->mBitmap.get();
    jobject jBitDat = m_jenv->CallStaticObjectMethod(C(), method, jBitmap);

    //IMPORTANT: color byte order conversion.
    MData::ptr bitDat = m_cpp_ints << jBitDat; {
        int *pixels = (int *)bitDat->bytes();
        int  count  = bitDat->length() / 4;
        MColorTransform((MEarlyARGB *)pixels, (MColorRGBA *)pixels, count);
    }

    return bitDat;
}

MSize::ptr MAndrdImageFactory::onGetPixelSize(const MImage::ptr &image) {
    static jmethodID wMethod = m_jenv->GetStaticMethodID(C(), "onGetPixelX", "(Landroid/graphics/Bitmap;)F");
    static jmethodID hMethod = m_jenv->GetStaticMethodID(C(), "onGetPixelY", "(Landroid/graphics/Bitmap;)F");

    jobject jBitmap = ((MAndrdImage *)image.get())->mBitmap.get();
    float   width   = m_jenv->CallStaticFloatMethod(C(), wMethod, jBitmap);
    float   height  = m_jenv->CallStaticFloatMethod(C(), hMethod, jBitmap);

    return MSize::from(width, height);
}
