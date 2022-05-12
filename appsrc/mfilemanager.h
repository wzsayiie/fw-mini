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

    //app specific directories.
    std::string documentDirectory ();
    std::string temporaryDirectory();
    
private:
    std::string mDocumentDirectory ;
    std::string mTemporaryDirectory;
};

//bundle:

M_HOST_IMPLEMENT_CLASS
m_class(MBundle, MObject) {
public:
    static constexpr const char *const PrivateDirectoryName = "mini";
    static constexpr const char *const BundleDirectoryName  = "mini.bundle";
    
public:
    M_HOST_CALL_FUNCTION static void setInstance(const MBundle::ptr &obj);
    static MBundle *instance();

public:
    virtual MVector<uint8_t>::ptr loadAsset(const std::string &path);

    virtual std::string documentDirectory ();
    virtual std::string temporaryDirectory();

private:
    static MBundle::ptr sInstance;
};
