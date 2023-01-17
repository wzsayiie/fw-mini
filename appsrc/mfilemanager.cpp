#include "mfilemanager.h"
#include <filesystem>
#include "dfile.h"
#include "rdefine.h"

//file manager:

define_reflectable_class_function(MFileManager, instance, "getter")
MFileManager *MFileManager::instance() {
    static MFileManager::ptr obj = MFileManager::create();
    return obj.get();
}

define_reflectable_class_function(MFileManager, bytesFromBundle, "args:path")
MData::ptr MFileManager::bytesFromBundle(const std::string &path) {
    if (path.empty()) {
        return nullptr;
    }

    MData::ptr data = MBundle::instance()->loadResource(path);
    if (!data || data->empty()) {
        return nullptr;
    }

    return data;
}

define_reflectable_class_function(MFileManager, bytesFromFile, "args:path")
MData::ptr MFileManager::bytesFromFile(const std::string &path) {
    auto data = MData::create();
    
    dash::read_file(path.c_str(), [&](int size) {
        data->resize(size);
        return data->bytes();
    });

    return data->empty() ? nullptr : data;
}

define_reflectable_class_function(MFileManager, u8stringFromBundle, "args:path")
std::string MFileManager::u8stringFromBundle(const std::string &path) {
    if (path.empty()) {
        return "";
    }

    MData::ptr data = MBundle::instance()->loadResource(path);
    if (!data || data->empty()) {
        return "";
    }

    auto bgn = (char *)data->bytes();
    auto end = (char *)data->bytesEnd();
    return std::string(bgn, end);
}

define_reflectable_class_function(MFileManager, u8stringFromFile, "args:path")
std::string MFileManager::u8stringFromFile(const std::string &path) {
    std::string str;

    dash::read_file(path.c_str(), [&](int size) {
        str.resize((size_t)(size + 1));
        return str.data();
    });

    return str;
}

define_reflectable_class_function(MFileManager, writeBytesToFile, "args:bytes,path")
void MFileManager::writeBytesToFile(const MData::ptr &data, const std::string &path) {
    if (data) {
        dash::write_file(path.c_str(), data->bytes(), data->length());
    } else {
        //clear the file.
        dash::write_file(path.c_str(), nullptr, 0);
    }
}

define_reflectable_class_function(MFileManager, writeU8StringToFile, "args:str,path")
void MFileManager::writeU8StringToFile(const std::string &string, const std::string &path) {
    dash::write_file(path.c_str(), string.c_str(), (int)string.size());
}

define_reflectable_class_function(MFileManager, contentsOfDirectory, "args:path")
MVector<std::string>::ptr MFileManager::contentsOfDirectory(const std::string &path) {
    auto contents = MVector<std::string>::create();

    std::filesystem::directory_iterator iterator(path);
    for (auto &entry : iterator) {
        std::string item = entry.path().string();
        contents->push_back(item);
    }

    return contents;
}

define_reflectable_class_function(MFileManager, fileExistsAt, "args:path")
bool MFileManager::fileExistsAt(const std::string &path) {
    if (!path.empty()) {
        return std::filesystem::exists(path);
    }
    return false;
}

define_reflectable_class_function(MFileManager, directoryExistsAt, "args:path")
bool MFileManager::directoryExistsAt(const std::string &path) {
    if (!path.empty()) {
        return std::filesystem::is_directory(path);
    }
    return false;
}

define_reflectable_class_function(MFileManager, createDirectory, "args:path")
void MFileManager::createDirectory(const std::string &path) {
    if (!path.empty()) {
        std::filesystem::create_directories(path);
    }
}

define_reflectable_class_function(MFileManager, removePath, "args:path")
void MFileManager::removePath(const std::string &path) {
    if (!path.empty()) {
        std::filesystem::remove_all(path);
    }
}

//bundle:

define_reflectable_class_const(MBundle, PrivateDirectoryName  )
define_reflectable_class_const(MBundle, ResBundleDirectoryName)

MBundle::ptr MBundle::sInstance;

define_reflectable_class_function(MBundle, setInstance, "setter;args:obj")
void MBundle::setInstance(const MBundle::ptr &obj) {
    sInstance = obj;
}

define_reflectable_class_function(MBundle, instance, "getter")
MBundle *MBundle::instance() {
    if (!sInstance) {
        sInstance = MBundle::create();
    }
    return sInstance.get();
}

define_reflectable_class_function(MBundle, getDocumentFilePath , "args:filename")
define_reflectable_class_function(MBundle, getTemporaryFilePath, "args:filename")

static std::string GetP(const std::string &dir, const std::string &file) {
    const char *item = file.c_str();
    while (*item == '\\' || *item == '/') {
        item += 1;
    }

    if (*item) {
        return dir + '/' + item;
    } else {
        return "";
    }
}

std::string MBundle::getDocumentFilePath (const std::string &n) { return GetP(documentDirectory (), n); }
std::string MBundle::getTemporaryFilePath(const std::string &n) { return GetP(temporaryDirectory(), n); }

define_reflectable_class_function(MBundle, loadResource, "args:path")
MData::ptr MBundle::loadResource(const std::string &path) {
    if (!path.empty()) {
        return onLoadResource(path);
    }
    return nullptr;
}

define_reflectable_class_function(MBundle, resBundleDirectory, "getter")
define_reflectable_class_function(MBundle, documentDirectory , "getter")
define_reflectable_class_function(MBundle, temporaryDirectory, "getter")

static std::string GetD(MBundle *_this, std::string *dir, std::string (MBundle::*getter)()) {
    if (dir->empty()) {
        *dir = (_this->*getter)();
    }
    return *dir;
}

std::string MBundle::resBundleDirectory() { return GetD(this, &mResBundleDirectory, &MBundle::onGetResBundleDirectory); }
std::string MBundle::documentDirectory () { return GetD(this, &mDocumentDirectory , &MBundle::onGetDocumentDirectory ); }
std::string MBundle::temporaryDirectory() { return GetD(this, &mTemporaryDirectory, &MBundle::onGetTemporaryDirectory); }

MData::ptr MBundle::onLoadResource(const std::string &path) {
    return nullptr;
}

std::string MBundle::onGetResBundleDirectory() { return ""; }
std::string MBundle::onGetDocumentDirectory () { return ""; }
std::string MBundle::onGetTemporaryDirectory() { return ""; }
