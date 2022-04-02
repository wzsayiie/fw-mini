#include "mfilemanager.h"
#include "mencode.h"
#include <filesystem>
#include <fstream>

//normal path:
//

//on windows platform, "\" and "/" are both path separators.
//but for unix and its derivatives, "\" can be included in filename.
//use normalized paths to avoid ambiguity.

static std::string NormalizePath(const std::string &path) {
    std::string normal = path;
    for (auto it = normal.begin(); it != normal.end(); ++it) {
        if (*it == '\\') {
            *it = '/';
        }
    }
    return normal;
}

//file manager:
//

define_reflectable_class_function(MFileManager, sharedObject)
MFileManager *MFileManager::sharedObject() {
    static MFileManager::ptr obj = MFileManager::create();
    return obj.get();
}

define_reflectable_class_function(MFileManager, bytesFromBundle, "args:path;")
MVector<uint8_t>::ptr MFileManager::bytesFromBundle(const std::string &path) {
    if (!path.empty()) {
        std::string normalPath = NormalizePath(path);
        return MBundle::sharedObject()->loadAsset(normalPath);
    }
    return nullptr;
}

define_reflectable_class_function(MFileManager, bytesFromFile)
MVector<uint8_t>::ptr MFileManager::bytesFromFile(const std::string &path) {
    if (path.empty()) {
        return nullptr;
    }

    std::string normalPath = NormalizePath(path);
    std::ifstream file(normalPath, std::ios::binary);
    if (!file.is_open()) {
        return nullptr;
    }

    size_t size = 0; {
        file.seekg(0, std::ios::end);
        size = (size_t)file.tellg();
        file.seekg(0, std::ios::beg);
    }

    auto bytes = MVector<uint8_t>::create();
    bytes->vector.resize(size);
    file.read((char *)bytes->vector.data(), size);
    return bytes;
}

define_reflectable_class_function(MFileManager, u8stringFromBundle, "args:path;")
std::string MFileManager::u8stringFromBundle(const std::string &path) {
    MVector<uint8_t>::ptr bytes = bytesFromBundle(path);
    if (!bytes || bytes->vector.empty()) {
        return "";
    }

    auto bgn = (char *)bytes->vector.data();
    auto end = (char *)bytes->vector.data() + bytes->vector.size();
    return std::string(bgn, end);
}

define_reflectable_class_function(MFileManager, u8stringFromFile, "args:path;")
std::string MFileManager::u8stringFromFile(const std::string &path) {
    MVector<uint8_t>::ptr bytes = bytesFromFile(path);
    if (!bytes || bytes->vector.empty()) {
        return "";
    }

    auto bgn = (char *)bytes->vector.data();
    auto end = (char *)bytes->vector.data() + bytes->vector.size();
    return std::string(bgn, end);
}

define_reflectable_class_function(MFileManager, writeBytesToFile, "args:bytes,path;")
void MFileManager::writeBytesToFile(const MVector<uint8_t>::ptr &bytes, const std::string &path) {
    if (path.empty()) {
        return;
    }

    std::string normalString = NormalizePath(path);
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return;
    }

    if (bytes && !bytes->vector.empty()) {
        file.write((const char *)bytes->vector.data(), bytes->vector.size());
    } else {
        file.write("", 0);
    }
}

define_reflectable_class_function(MFileManager, writeU8StringToFile, "args:str,path;")
void MFileManager::writeU8StringToFile(const std::string &string, const std::string &path) {
    auto bgn = (uint8_t *)string.c_str();
    auto end = (uint8_t *)string.c_str() + string.size();

    auto bytes = MVector<uint8_t>::create();
    bytes->vector.insert(bytes->vector.end(), bgn, end);
    writeBytesToFile(bytes, path);
}

define_reflectable_class_function(MFileManager, contentsOfDirectory, "args:path;")
MVector<std::string>::ptr MFileManager::contentsOfDirectory(const std::string &path) {
    auto contents = MVector<std::string>::create();

    std::string normalPath = NormalizePath(path);
    std::filesystem::directory_iterator iterator(normalPath);
    for (auto &entry : iterator) {
        std::string item = entry.path().string();
        std::string norm = NormalizePath(item);
        contents->push_back(norm);
    }

    return contents;
}

define_reflectable_class_function(MFileManager, fileExistsAt, "args:path;")
bool MFileManager::fileExistsAt(const std::string &path) {
    if (!path.empty()) {
        std::string normalPath = NormalizePath(path);
        return std::filesystem::exists(normalPath);
    }
    return false;
}

define_reflectable_class_function(MFileManager, directoryExistsAt, "args:path;")
bool MFileManager::directoryExistsAt(const std::string &path) {
    if (!path.empty()) {
        std::string normalPath = NormalizePath(path);
        return std::filesystem::is_directory(normalPath);
    }
    return false;
}

define_reflectable_class_function(MFileManager, createDirectory, "args:path;")
void MFileManager::createDirectory(const std::string &path) {
    if (!path.empty()) {
        std::string normalPath = NormalizePath(path);
        std::filesystem::create_directories(normalPath);
    }
}

define_reflectable_class_function(MFileManager, removePath, "args:path;")
void MFileManager::removePath(const std::string &path) {
    if (!path.empty()) {
        std::string normalPath = NormalizePath(path);
        std::filesystem::remove_all(normalPath);
    }
}

define_reflectable_class_function(MFileManager, documentDirectory)
std::string MFileManager::documentDirectory() {
    return MBundle::sharedObject()->documentDirectory();
}

define_reflectable_class_function(MFileManager, temporaryDirectory)
std::string MFileManager::temporaryDirectory() {
    return MBundle::sharedObject()->temporaryDirectory();
}

//bundle:
//

MBundle::ptr MBundle::sSharedObject;

define_reflectable_class_function(MBundle, setSharedObject, "args:bundle;")
void MBundle::setSharedObject(const MBundle::ptr &obj) {
    sSharedObject = obj;
}

define_reflectable_class_function(MBundle, sharedObject)
MBundle *MBundle::sharedObject() {
    if (!sSharedObject) {
        sSharedObject = MBundle::create();
    }
    return sSharedObject.get();
}

define_reflectable_class_function(MBundle, loadAsset, "args:path;")
MVector<uint8_t>::ptr MBundle::loadAsset(const std::string &path) {
    implement_injectable_function((MVector<uint8_t>::ptr), path);
    return nullptr;
}

define_reflectable_class_function(MBundle, documentDirectory)
std::string MBundle::documentDirectory() {
    implement_injectable_function((std::string))
    return "";
}

define_reflectable_class_function(MBundle, temporaryDirectory)
std::string MBundle::temporaryDirectory() {
    implement_injectable_function((std::string))
    return "";
}
