#include "AndroidApi.h"

static JNIEnv *sJniEnv = nullptr;
static jclass  sJClass = nullptr;

//------------------------------------------------------------------------------
//MAndroidImage:

MAndroidImage::MAndroidImage(jobject nativeImage)
{
    mNativeImage = sJniEnv->NewGlobalRef(nativeImage);
}

MAndroidImage::~MAndroidImage()
{
    sJniEnv->DeleteGlobalRef(mNativeImage);
}

jobject MAndroidImage::nativeImage()
{
    return mNativeImage;
}

//------------------------------------------------------------------------------
//api assist:

static jstring JNewString(MString *src)
{
    if (!src)
    {
        return nullptr;
    }

    auto chars = (const jchar *)MStringU16Chars(src);
    auto size  = (jsize)MStringU16Size(src);

    return sJniEnv->NewString(chars, size);
}

static jbyteArray JNewByteArray(MData *src)
{
    if (!src)
    {
        return nullptr;
    }

    auto bytes = (const jbyte *)MDataBytes(src);
    auto size  = (jsize)MDataSize(src);

    jbyteArray dst = sJniEnv->NewByteArray(size);
    sJniEnv->SetByteArrayRegion(dst, 0, size, bytes);
    return dst;
}

static MString *CreateString(jstring src)
{
    if (!src)
    {
        return nullptr;
    }

    //use GetStringUTFChars (the return string of GetStringChars has no terminator '\0').
    const char *chars = sJniEnv->GetStringUTFChars(src, nullptr);

    MString *dst = MStringCreateU8(chars);
    sJniEnv->ReleaseStringUTFChars(src, chars);

    return dst;
}

static MData *CreateData(jbyteArray src)
{
    if (!src)
    {
        return nullptr;
    }

    jbyte *bytes = sJniEnv->GetByteArrayElements(src, nullptr);
    jsize  size  = sJniEnv->GetArrayLength(src);

    MData *dst = MDataCreate((const uint8_t *)bytes, (int)size);
    sJniEnv->ReleaseByteArrayElements(src, bytes, 0);

    return dst;
}

struct JLocalRefDeleteHelper
{
    template<typename T> std::shared_ptr<T> operator<<(T *object)
    {
        return std::shared_ptr<T>(object, [](T *a) { sJniEnv->DeleteLocalRef(a); });
    }
};
#define j_auto_delete JLocalRefDeleteHelper()<<

//------------------------------------------------------------------------------
//android api:

static jmethodID sPrintMessage      = nullptr;
static jmethodID sCopyBundleAsset   = nullptr;
static jmethodID sCreateImage       = nullptr;
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
    auto string = j_auto_delete JNewString(text);
    sJniEnv->CallStaticVoidMethod(sJClass, sPrintMessage, string.get());
}

static MData *CopyBundleAsset(MString *path)
{
    auto assetPath = j_auto_delete JNewString(path);
    auto assetData = j_auto_delete (jbyteArray)sJniEnv->CallStaticObjectMethod(sJClass, sCopyBundleAsset, assetPath.get());
    return CreateData(assetData.get());
}

static MImage *CreateImage(MData *data)
{
    auto imageData   = j_auto_delete JNewByteArray(data);
    auto imageObject = j_auto_delete sJniEnv->CallStaticObjectMethod(sJClass, sCreateImage, imageData.get());

    if (imageObject)
    {
        return new MAndroidImage(imageObject.get());
    }
    return nullptr;
}

static MString *CopyDocumentPath()
{
    auto path = j_auto_delete (jstring)sJniEnv->CallStaticObjectMethod(sJClass, sCopyDocumentPath);
    return CreateString(path.get());
}

static MString *CopyCachePath()
{
    auto path = j_auto_delete (jstring)sJniEnv->CallStaticObjectMethod(sJClass, sCopyCachePath);
    return CreateString(path.get());
}

static MString *CopyTemporaryPath()
{
    auto path = j_auto_delete (jstring)sJniEnv->CallStaticObjectMethod(sJClass, sCopyTemporaryPath);
    return CreateString(path.get());
}

static bool MakeDirectory(MString *path)
{
    auto dirPath = j_auto_delete JNewString(path);
    return sJniEnv->CallStaticBooleanMethod(sJClass, sMakeDirectory, dirPath.get());
}

MArray *CopyPathSubItems(MString *path)
{
    auto dirPath  = j_auto_delete JNewString(path);
    auto subFiles = j_auto_delete (jobjectArray)sJniEnv->CallStaticObjectMethod(sJClass, sCopyPathSubItems, dirPath.get());
    if (subFiles == nullptr)
    {
        return nullptr;
    }

    MArray *array = MArrayCreate();

    jsize count = sJniEnv->GetArrayLength(subFiles.get());
    for (jsize index = 0; index < count; ++index)
    {
        auto sub = (jstring)sJniEnv->GetObjectArrayElement(subFiles.get(), index);

        MStringRef item = m_auto_release CreateString(sub);
        MArrayAppend(array, item.get());
    }

    return array;
}

static void RemovePath(MString *path)
{
    auto targetPath = j_auto_delete JNewString(path);
    sJniEnv->CallStaticVoidMethod(sJClass, sRemovePath, targetPath.get());
}

static bool PathExists(MString *path)
{
    auto targetPath = j_auto_delete JNewString(path);
    return sJniEnv->CallStaticBooleanMethod(sJClass, sPathExists, targetPath.get());
}

static bool DirectoryExists(MString *path)
{
    auto targetPath = j_auto_delete JNewString(path);
    return sJniEnv->CallStaticBooleanMethod(sJClass, sDirectoryExists, targetPath.get());
}

static bool FileExists(MString *path)
{
    auto targetPath = j_auto_delete JNewString(path);
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
    sCopyDocumentPath  = env->GetStaticMethodID(sJClass, "copyDocumentPath" , "()Ljava/lang/String;");
    sCopyCachePath     = env->GetStaticMethodID(sJClass, "copyCachePath"    , "()Ljava/lang/String;");
    sCopyTemporaryPath = env->GetStaticMethodID(sJClass, "copyTemporaryPath", "()Ljava/lang/String;");
    sMakeDirectory     = env->GetStaticMethodID(sJClass, "makeDirectory"    , "(Ljava/lang/String;)Z");
    sCopyPathSubItems  = env->GetStaticMethodID(sJClass, "copyPathSubItems" , "(Ljava/lang/String;)[Ljava/lang/String;");
    sRemovePath        = env->GetStaticMethodID(sJClass, "removePath"       , "(Ljava/lang/String;)V");
    sPathExists        = env->GetStaticMethodID(sJClass, "pathExists"       , "(Ljava/lang/String;)Z");
    sDirectoryExists   = env->GetStaticMethodID(sJClass, "directoryExists"  , "(Ljava/lang/String;)Z");
    sFileExists        = env->GetStaticMethodID(sJClass, "fileExists"       , "(Ljava/lang/String;)Z");

    _MSetApiPrintMessage     (PrintMessage     );
    _MSetApiCopyBundleAsset  (CopyBundleAsset  );
    _MSetApiCreateImage      (CreateImage      );
    _MSetApiCopyDocumentPath (CopyDocumentPath );
    _MSetApiCopyCachePath    (CopyCachePath    );
    _MSetApiCopyTemporaryPath(CopyTemporaryPath);
    _MSetApiMakeDirectory    (MakeDirectory    );
    _MSetApiCopyPathSubItems (CopyPathSubItems );
    _MSetApiRemovePath       (RemovePath       );
    _MSetApiPathExists       (PathExists       );
    _MSetApiDirectoryExists  (DirectoryExists  );
    _MSetApiFileExists       (FileExists       );
}

extern "C" JNIEXPORT jstring JNICALL
Java_src_app_mini_AndroidApi_assetBundleName(JNIEnv *env, jclass)
{
    jsize size = 0;
    for (const char16_t *it = _MAssetBundleU16Name; *it; ++it)
    {
        size += 1;
    }
    return env->NewString((const jchar *)_MAssetBundleU16Name, size);
}
