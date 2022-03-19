#include "mfilemanager.h"
#include "mencode.h"
#include <filesystem>

//file manager:
//

define_reflectable_class_function(MFileManager, sharedObject)
MFileManager *MFileManager::sharedObject() {
    static MFileManager::ptr obj = MFileManager::create();
    return obj.get();
}

define_reflectable_class_function(MFileManager, bytesFromBundle, "args:path;")
MVector<uint8_t>::ptr MFileManager::bytesFromBundle(const std::string &path) {
    return MBundle::sharedObject()->loadAsset(path);
}

define_reflectable_class_function(MFileManager, bytesFromFile)
MVector<uint8_t>::ptr MFileManager::bytesFromFile(const std::string &path) {
    return nullptr;
}

define_reflectable_class_function(MFileManager, textFromBundle, "args:path;")
std::string MFileManager::textFromBundle(const std::string &path) {
    return "";
}

define_reflectable_class_function(MFileManager, textFromFile, "args:path;")
std::string MFileManager::textFromFile(const std::string &path) {
    return "";
}

define_reflectable_class_function(MFileManager, writeBytesToFile, "args:bytes,path;")
void MFileManager::writeBytesToFile(const MVector<uint8_t>::ptr &bytes, const std::string &path) {
}

define_reflectable_class_function(MFileManager, writeTextToFile, "args:text,path;")
void MFileManager::writeTextToFile(const std::string &text, const std::string &path) {
}

define_reflectable_class_function(MFileManager, contentsOfDirectory, "args:path;")
MVector<std::string>::ptr MFileManager::contentsOfDirectory(const std::string &path) {
    auto contents = MVector<std::string>::create();

    std::filesystem::directory_iterator iterator(path);
    for (auto &entry : iterator) {
        const std::string &name = entry.path().string();
        contents->push_back(name);
    }

    return contents;
}

define_reflectable_class_function(MFileManager, directoryExistsAt, "args:path;")
bool MFileManager::directoryExistsAt(const std::string &path) {
    return std::filesystem::is_directory(path.c_str());
}

define_reflectable_class_function(MFileManager, fileExistsAt, "args:path;")
bool MFileManager::fileExistsAt(const std::string &path) {
    if (!std::filesystem::exists(path.c_str())) {
        return false;
    }
    if (std::filesystem::is_directory(path.c_str())) {
        return false;
    }
    return true;
}

define_reflectable_class_function(MFileManager, createDirectory, "args:path;")
void MFileManager::createDirectory(const std::string &path) {
    std::filesystem::create_directories(path.c_str());
}

define_reflectable_class_function(MFileManager, removePath, "args:path;")
void MFileManager::removePath(const std::string &path) {
    std::filesystem::remove_all(path.c_str());
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
