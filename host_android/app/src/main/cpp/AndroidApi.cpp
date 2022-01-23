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
static jmethodID sMakeDirectory     = nullptr;
static jmethodID sCopyPathSubItems  = nullptr;
static jmethodID sRemovePath        = nullptr;
static jmethodID sPathExists        = nullptr;
static jmethodID sDirectoryExists   = nullptr;
static jmethodID sFileExists        = nullptr;

static void PrintMessage(MString *text)
{
    auto string = j_auto_delete(sJniEnv) JNewString(sJniEnv, text);
    sJniEnv->CallStaticVoidMethod(sJClass, sPrintMessage, string.get());
}

static MData *CopyBundleAsset(MString *path)
{
    auto assetPath = j_auto_delete(sJniEnv) JNewString(sJniEnv, path);
    auto assetData = j_auto_delete(sJniEnv) (jbyteArray)sJniEnv->CallStaticObjectMethod(sJClass, sCopyBundleAsset, assetPath.get());
    return MDataCopyJByteArray(sJniEnv, assetData.get());
}

static MImage *CreateImage(MData *data)
{
    auto imageData = j_auto_delete(sJniEnv) JNewByteArray(sJniEnv, data);
    auto javaImage = j_auto_delete(sJniEnv) sJniEnv->CallStaticObjectMethod(sJClass, sCreateImage, imageData.get());

    if (javaImage)
    {
        return new MAndroidImage(javaImage.get());
    }
    return nullptr;
}

static MImage *CreateBitmapImage(MData *data, int width, int height)
{
    MDataRef pixelData  = m_auto_release MDataCopy(MDataBytes(data), MDataSize(data));
    uint8_t *pixelBytes = MDataBytes(pixelData.get());
    MConvertColors(width * height, (MColorPattern *)pixelBytes, (MAndroidColorPattern *)pixelBytes);

    auto imageData = j_auto_delete(sJniEnv) JNewIntArray(sJniEnv, pixelData.get());
    auto javaImage = j_auto_delete(sJniEnv) sJniEnv->CallStaticObjectMethod(sJClass, sCreateBitmapImage, imageData.get(), width, height);

    if (javaImage)
    {
        return new MAndroidImage(javaImage.get());
    }
    return nullptr;
}

static MData *CopyImageBitmap(MImage *image)
{
    auto javaImage = ((MAndroidImage *)image)->javaImage();
    auto imageData = j_auto_delete(sJniEnv) (jintArray)sJniEnv->CallStaticObjectMethod(sJClass, sCopyImageBitmap, javaImage);

    MData *data = MDataCopyJIntArray(sJniEnv, imageData.get());

    uint8_t *bytes = MDataBytes(data);
    int size = MDataSize(data);
    MConvertColors(size / 4, (MAndroidColorPattern *)bytes, (MColorPattern *)bytes);

    return data;
}

static int ImagePixelWidth(MImage *image)
{
    auto javaImage = ((MAndroidImage *)image)->javaImage();
    return sJniEnv->CallStaticIntMethod(sJClass, sImagePixelWidth, javaImage);
}

static int ImagePixelHeight(MImage *image)
{
    auto javaImage = ((MAndroidImage *)image)->javaImage();
    return sJniEnv->CallStaticIntMethod(sJClass, sImagePixelHeight, javaImage);
}

static MString *CopyDocumentPath()
{
    auto path = j_auto_delete(sJniEnv) (jstring)sJniEnv->CallStaticObjectMethod(sJClass, sCopyDocumentPath);
    return MStringCopyJString(sJniEnv, path.get());
}

static MString *CopyCachePath()
{
    auto path = j_auto_delete(sJniEnv) (jstring)sJniEnv->CallStaticObjectMethod(sJClass, sCopyCachePath);
    return MStringCopyJString(sJniEnv, path.get());
}

static MString *CopyTemporaryPath()
{
    auto path = j_auto_delete(sJniEnv) (jstring)sJniEnv->CallStaticObjectMethod(sJClass, sCopyTemporaryPath);
    return MStringCopyJString(sJniEnv, path.get());
}

static bool MakeDirectory(MString *path)
{
    auto dirPath = j_auto_delete(sJniEnv) JNewString(sJniEnv, path);
    return sJniEnv->CallStaticBooleanMethod(sJClass, sMakeDirectory, dirPath.get());
}

MArray *CopyPathSubItems(MString *path)
{
    auto dirPath  = j_auto_delete(sJniEnv) JNewString(sJniEnv, path);
    auto subFiles = j_auto_delete(sJniEnv) (jobjectArray)sJniEnv->CallStaticObjectMethod(sJClass, sCopyPathSubItems, dirPath.get());
    if (subFiles == nullptr)
    {
        return nullptr;
    }

    MArray *array = MArrayCreate();

    jsize count = sJniEnv->GetArrayLength(subFiles.get());
    for (jsize index = 0; index < count; ++index)
    {
        auto sub = (jstring)sJniEnv->GetObjectArrayElement(subFiles.get(), index);

        MStringRef item = m_auto_release MStringCopyJString(sJniEnv, sub);
        MArrayAppend(array, item.get());
    }

    return array;
}

static void RemovePath(MString *path)
{
    auto targetPath = j_auto_delete(sJniEnv) JNewString(sJniEnv, path);
    sJniEnv->CallStaticVoidMethod(sJClass, sRemovePath, targetPath.get());
}

static bool PathExists(MString *path)
{
    auto targetPath = j_auto_delete(sJniEnv) JNewString(sJniEnv, path);
    return sJniEnv->CallStaticBooleanMethod(sJClass, sPathExists, targetPath.get());
}

static bool DirectoryExists(MString *path)
{
    auto targetPath = j_auto_delete(sJniEnv) JNewString(sJniEnv, path);
    return sJniEnv->CallStaticBooleanMethod(sJClass, sDirectoryExists, targetPath.get());
}

static bool FileExists(MString *path)
{
    auto targetPath = j_auto_delete(sJniEnv) JNewString(sJniEnv, path);
    return sJniEnv->CallStaticBooleanMethod(sJClass, sFileExists, targetPath.get());
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
    sMakeDirectory     = env->GetStaticMethodID(sJClass, "makeDirectory"    , "(Ljava/lang/String;)Z");
    sCopyPathSubItems  = env->GetStaticMethodID(sJClass, "copyPathSubItems" , "(Ljava/lang/String;)[Ljava/lang/String;");
    sRemovePath        = env->GetStaticMethodID(sJClass, "removePath"       , "(Ljava/lang/String;)V");
    sPathExists        = env->GetStaticMethodID(sJClass, "pathExists"       , "(Ljava/lang/String;)Z");
    sDirectoryExists   = env->GetStaticMethodID(sJClass, "directoryExists"  , "(Ljava/lang/String;)Z");
    sFileExists        = env->GetStaticMethodID(sJClass, "fileExists"       , "(Ljava/lang/String;)Z");

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
    _MSetApi_MakeDirectory    (MakeDirectory    );
    _MSetApi_CopyPathSubItems (CopyPathSubItems );
    _MSetApi_RemovePath       (RemovePath       );
    _MSetApi_PathExists       (PathExists       );
    _MSetApi_DirectoryExists  (DirectoryExists  );
    _MSetApi_FileExists       (FileExists       );
}

extern "C" JNIEXPORT jstring JNICALL
Java_src_app_mini_AndroidApi_assetBundleName(JNIEnv *env, jclass)
{
    MStringRef string = m_auto_release MStringCreateU16(MAssetBundleU16Name);
    return JNewString(env, string.get());
}
