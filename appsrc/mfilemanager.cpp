#include "mfilemanager.h"
#include <filesystem>
#include "dfile.h"
#include "mbundle.h"
#include "rdefine.h"

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
