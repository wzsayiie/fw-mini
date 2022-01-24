#include "AndroidApi.h"

static JNIEnv *sJniEnv = nullptr;
static jclass  sJClass = nullptr;

//------------------------------------------------------------------------------
//MAndroidImage:

MAndroidImage::MAndroidImage(jobject javaImage)
{
    mJavaImage = sJniEnv->NewGlobalRef(javaImage);
}

MAndroidImage::~MAndroidImage()
{
    sJniEnv->DeleteGlobalRef(mJavaImage);
}

jobject MAndroidImage::javaImage()
{
    return mJavaImage;
}

//------------------------------------------------------------------------------
//android api:

static jmethodID sPrintMessage      = nullptr;
static jmethodID sCopyBundleAsset   = nullptr;
static jmethodID sCreateImage       = nullptr;
static jmethodID sCreateBitmapImage = nullptr;
static jmethodID sCopyImageBitmap   = nullptr;
static jmethodID sImagePixelWidth   = nullptr;
static jmethodID sImagePixelHeight  = nullptr;
static jmethodID sCopyDocumentPath  = nullptr;
static jmethodID sCopyCachePath     = nullptr;
static jmethodID sCopyTemporaryPath = nullptr;

static void PrintMessage(MString *text)
{
    std::shared_ptr<_jstring> string = j_auto_delete(sJniEnv) JNewString(sJniEnv, text);
    sJniEnv->CallStaticVoidMethod(sJClass, sPrintMessage, string.get());
}

static MData *CopyBundleAsset(MString *path)
{
    std::shared_ptr<_jstring   > assetPath = j_auto_delete(sJniEnv) JNewString(sJniEnv, path);
    std::shared_ptr<_jbyteArray> assetData = j_auto_delete(sJniEnv) (jbyteArray)sJniEnv->CallStaticObjectMethod(
        sJClass, sCopyBundleAsset, assetPath.get());

    return MDataCopyJByteArray(sJniEnv, assetData.get());
}

static MImage *CreateImage(MData *data)
{
    std::shared_ptr<_jbyteArray> imageData = j_auto_delete(sJniEnv) JNewByteArray(sJniEnv, data);
    std::shared_ptr<_jobject   > javaImage = j_auto_delete(sJniEnv) sJniEnv->CallStaticObjectMethod(
        sJClass, sCreateImage, imageData.get());

    return new MAndroidImage(javaImage.get());
}

static MImage *CreateBitmapImage(MData *data, int width, int height)
{
    MDataRef pixelData  = m_auto_release MDataCopy(MDataBytes(data), MDataSize(data));
    uint8_t *pixelBytes = MDataBytes(pixelData.get());
    MConvertColors(width * height, (MColorPattern *)pixelBytes, (MAndroidColorPattern *)pixelBytes);

    std::shared_ptr<_jintArray> imageData = j_auto_delete(sJniEnv) JNewIntArray(sJniEnv, pixelData.get());
    std::shared_ptr<_jobject  > javaImage = j_auto_delete(sJniEnv) sJniEnv->CallStaticObjectMethod(
        sJClass, sCreateBitmapImage, imageData.get(), width, height);

    return new MAndroidImage(javaImage.get());
}

static MData *CopyImageBitmap(MImage *image)
{
    _jobject* javaImage = ((MAndroidImage *)image)->javaImage();
    std::shared_ptr<_jintArray> imageData = j_auto_delete(sJniEnv) (jintArray)sJniEnv->CallStaticObjectMethod(
        sJClass, sCopyImageBitmap, javaImage);

    MData *data = MDataCopyJIntArray(sJniEnv, imageData.get());

    uint8_t *bytes = MDataBytes(data);
    int size = MDataSize(data);
    MConvertColors(size / 4, (MAndroidColorPattern *)bytes, (MColorPattern *)bytes);

    return data;
}

static int ImagePixelWidth(MImage *image)
{
    jobject javaImage = ((MAndroidImage *)image)->javaImage();
    return sJniEnv->CallStaticIntMethod(sJClass, sImagePixelWidth, javaImage);
}

static int ImagePixelHeight(MImage *image)
{
    jobject javaImage = ((MAndroidImage *)image)->javaImage();
    return sJniEnv->CallStaticIntMethod(sJClass, sImagePixelHeight, javaImage);
}

static MString *CopyDocumentPath()
{
    std::shared_ptr<_jstring> path = j_auto_delete(sJniEnv) (jstring)sJniEnv->CallStaticObjectMethod(
        sJClass, sCopyDocumentPath);

    return MStringCopyJString(sJniEnv, path.get());
}

static MString *CopyCachePath()
{
    std::shared_ptr<_jstring> path = j_auto_delete(sJniEnv) (jstring)sJniEnv->CallStaticObjectMethod(
        sJClass, sCopyCachePath);

    return MStringCopyJString(sJniEnv, path.get());
}

static MString *CopyTemporaryPath()
{
    std::shared_ptr<_jstring> path = j_auto_delete(sJniEnv) (jstring)sJniEnv->CallStaticObjectMethod(
        sJClass, sCopyTemporaryPath);

    return MStringCopyJString(sJniEnv, path.get());
}

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_AndroidApi_registerAndroidApi(JNIEnv *env, jclass)
{
    jclass cls = env->FindClass("src/app/mini/AndroidApi");

    sJniEnv = env;
    sJClass = (jclass)env->NewGlobalRef(cls);

    sPrintMessage      = env->GetStaticMethodID(sJClass, "printMessage"     , "(Ljava/lang/String;)V");
    sCopyBundleAsset   = env->GetStaticMethodID(sJClass, "copyBundleAsset"  , "(Ljava/lang/String;)[B");
    sCreateImage       = env->GetStaticMethodID(sJClass, "createImage"      , "([B)Landroid/graphics/Bitmap;");
    sCreateBitmapImage = env->GetStaticMethodID(sJClass, "createBitmapImage", "([III)Landroid/graphics/Bitmap;");
    sCopyImageBitmap   = env->GetStaticMethodID(sJClass, "copyImageBitmap"  , "(Landroid/graphics/Bitmap;)[I");
    sImagePixelWidth   = env->GetStaticMethodID(sJClass, "imagePixelWidth"  , "(Landroid/graphics/Bitmap;)I");
    sImagePixelHeight  = env->GetStaticMethodID(sJClass, "imagePixelHeight" , "(Landroid/graphics/Bitmap;)I");
    sCopyDocumentPath  = env->GetStaticMethodID(sJClass, "copyDocumentPath" , "()Ljava/lang/String;");
    sCopyCachePath     = env->GetStaticMethodID(sJClass, "copyCachePath"    , "()Ljava/lang/String;");
    sCopyTemporaryPath = env->GetStaticMethodID(sJClass, "copyTemporaryPath", "()Ljava/lang/String;");

    _MSetApi_PrintMessage     (PrintMessage     );
    _MSetApi_CopyBundleAsset  (CopyBundleAsset  );
    _MSetApi_CreateImage      (CreateImage      );
    _MSetApi_CreateBitmapImage(CreateBitmapImage);
    _MSetApi_CopyImageBitmap  (CopyImageBitmap  );
    _MSetApi_ImagePixelWidth  (ImagePixelWidth  );
    _MSetApi_ImagePixelHeight (ImagePixelHeight );
    _MSetApi_CopyDocumentPath (CopyDocumentPath );
    _MSetApi_CopyCachePath    (CopyCachePath    );
    _MSetApi_CopyTemporaryPath(CopyTemporaryPath);
}

extern "C" JNIEXPORT jstring JNICALL
Java_src_app_mini_AndroidApi_assetBundleName(JNIEnv *env, jclass)
{
    MStringRef string = m_auto_release MStringCreateU16(MAssetBundleU16Name);
    return JNewString(env, string.get());
}
