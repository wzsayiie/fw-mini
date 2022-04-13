#include "mfilemanager.h"

//normal path:
//

//on windows platform, "\" and "/" are both path separators.
//but for unix and its derivatives, "\" can be included in filename.
//use normalized paths to avoid ambiguity.

struct PathNormalizer {
    std::string operator<<(const std::string &path) {
        std::string normal = path;
        for (auto it = normal.begin(); it != normal.end(); ++it) {
            if (*it == '\\') {
                *it = '/';
            }
        }
        return normal;
    }
};

#define m_normal_path PathNormalizer()<<

//file manager:
//

define_reflectable_class_function(MFileManager, sharedObject)
MFileManager *MFileManager::sharedObject() {
    static MFileManager::ptr obj = MFileManager::create();
    return obj.get();
}

define_reflectable_class_function(MFileManager, bytesFromBundle, "args:path;")
MVector<uint8_t>::ptr MFileManager::bytesFromBundle(const std::string &path) {
    if (path.empty()) {
        return nullptr;
    }

    MVector<uint8_t>::ptr bytes = MBundle::sharedObject()->loadAsset(m_normal_path path);
    if (!bytes || bytes->vector.empty()) {
        return nullptr;
    }

    return bytes;
}

define_reflectable_class_function(MFileManager, bytesFromFile)
MVector<uint8_t>::ptr MFileManager::bytesFromFile(const std::string &path) {
    auto bytes = MVector<uint8_t>::create();
    
    std::string nPath = m_normal_path path;
    dash::read_file(nPath.c_str(), [&](int size) {
        bytes->vector.resize((size_t)size);
        return bytes->vector.data();
    });

    return bytes->vector.empty() ? nullptr : bytes;
}

define_reflectable_class_function(MFileManager, u8stringFromBundle, "args:path;")
std::string MFileManager::u8stringFromBundle(const std::string &path) {
    if (path.empty()) {
        return "";
    }

    MVector<uint8_t>::ptr bytes = MBundle::sharedObject()->loadAsset(m_normal_path path);
    if (!bytes || bytes->vector.empty()) {
        return "";
    }

    auto bgn = (char *)bytes->vector.data();
    auto end = (char *)bytes->vector.data() + bytes->vector.size();
    return std::string(bgn, end);
}

define_reflectable_class_function(MFileManager, u8stringFromFile, "args:path;")
std::string MFileManager::u8stringFromFile(const std::string &path) {
    std::string str;

    std::string nPath = m_normal_path path;
    dash::read_file(nPath.c_str(), [&](int size) {
        str.resize((size_t)(size + 1));
        return str.data();
    });

    return str;
}

define_reflectable_class_function(MFileManager, writeBytesToFile, "args:bytes,path;")
void MFileManager::writeBytesToFile(const MVector<uint8_t>::ptr &bytes, const std::string &path) {
    std::string nPath = m_normal_path path;
    if (bytes) {
        dash::write_file(nPath.c_str(), bytes->vector.data(), (int)bytes->vector.size());
    } else {
        //clear the file.
        dash::write_file(nPath.c_str(), nullptr, 0);
    }
}

define_reflectable_class_function(MFileManager, writeU8StringToFile, "args:str,path;")
void MFileManager::writeU8StringToFile(const std::string &string, const std::string &path) {
    std::string nPath = m_normal_path path;
    dash::write_file(nPath.c_str(), string.c_str(), (int)string.size());
}

define_reflectable_class_function(MFileManager, contentsOfDirectory, "args:path;")
MVector<std::string>::ptr MFileManager::contentsOfDirectory(const std::string &path) {
    auto contents = MVector<std::string>::create();

    std::filesystem::directory_iterator iterator(m_normal_path path);
    for (auto &entry : iterator) {
        std::string item = entry.path().string();
        contents->push_back(m_normal_path item);
    }

    return contents;
}

define_reflectable_class_function(MFileManager, fileExistsAt, "args:path;")
bool MFileManager::fileExistsAt(const std::string &path) {
    if (!path.empty()) {
        return std::filesystem::exists(m_normal_path path);
    }
    return false;
}

define_reflectable_class_function(MFileManager, directoryExistsAt, "args:path;")
bool MFileManager::directoryExistsAt(const std::string &path) {
    if (!path.empty()) {
        return std::filesystem::is_directory(m_normal_path path);
    }
    return false;
}

define_reflectable_class_function(MFileManager, createDirectory, "args:path;")
void MFileManager::createDirectory(const std::string &path) {
    if (!path.empty()) {
        std::filesystem::create_directories(m_normal_path path);
    }
}

define_reflectable_class_function(MFileManager, removePath, "args:path;")
void MFileManager::removePath(const std::string &path) {
    if (!path.empty()) {
        std::filesystem::remove_all(m_normal_path path);
    }
}

define_reflectable_class_function(MFileManager, documentDirectory)
std::string MFileManager::documentDirectory() {
    if (mDocumentDirectory.empty()) {
        mDocumentDirectory = m_normal_path MBundle::sharedObject()->documentDirectory();
    }
    return mDocumentDirectory;
}

define_reflectable_class_function(MFileManager, temporaryDirectory)
std::string MFileManager::temporaryDirectory() {
    if (mTemporaryDirectory.empty()) {
        mTemporaryDirectory = m_normal_path MBundle::sharedObject()->temporaryDirectory();
    }
    return mTemporaryDirectory;
}

//bundle:
//

define_reflectable_class_const(MBundle, PrivateDirectoryName)
define_reflectable_class_const(MBundle, BundleDirectoryName )

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
