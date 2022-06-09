#pragma once

#include "mobject.h"

//file manager:

m_class(MFileManager, MObject) {
public:
    static MFileManager *instance();

public:
    //data read and write:
    MVector<uint8_t>::ptr bytesFromBundle(const std::string &path);
    MVector<uint8_t>::ptr bytesFromFile  (const std::string &path);

    std::string u8stringFromBundle(const std::string &path);
    std::string u8stringFromFile  (const std::string &path);

    void writeBytesToFile   (const MVector<uint8_t>::ptr &bytes , const std::string &path);
    void writeU8StringToFile(const std::string           &string, const std::string &path);

    //path control:
    MVector<std::string>::ptr contentsOfDirectory(const std::string &path);

    bool fileExistsAt     (const std::string &path); //directory also recognized.
    bool directoryExistsAt(const std::string &path);
    void createDirectory  (const std::string &path);
    void removePath       (const std::string &path);
};

//bundle:

m_class(MBundle, MObject) {
public:
    static constexpr const char *PrivateDirectoryName = "mini";
    static constexpr const char *BundleDirectoryName  = "mini.bundle";

public:
    M_HOST_CALL static void setInstance(const MBundle::ptr &obj);
    static MBundle *instance();

public:
    MVector<uint8_t>::ptr loadAsset(const std::string &path);

    std::string bundleDirectory   ();
    std::string documentDirectory ();
    std::string temporaryDirectory();

protected:
    M_HOST_IMPL virtual MVector<uint8_t>::ptr onLoadAsset(const std::string &path);

    M_HOST_IMPL virtual std::string onGetBundleDirectory   ();
    M_HOST_IMPL virtual std::string onGetDocumentDirectory ();
    M_HOST_IMPL virtual std::string onGetTemporaryDirectory();

private:
    static MBundle::ptr sInstance;

private:
    std::string mBundleDirectory   ;
    std::string mDocumentDirectory ;
    std::string mTemporaryDirectory;
};
