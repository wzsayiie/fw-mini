#include "mfilemanager.h"

//normal path:

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

define_reflectable_class_function(MFileManager, instance, "getter")
MFileManager *MFileManager::instance() {
    static MFileManager::ptr obj = MFileManager::create();
    return obj.get();
}

define_reflectable_class_function(MFileManager, bytesFromBundle, "args:path")
MVector<uint8_t>::ptr MFileManager::bytesFromBundle(const std::string &path) {
    if (path.empty()) {
        return nullptr;
    }

    MVector<uint8_t>::ptr bytes = MBundle::instance()->loadResource(m_normal_path path);
    if (!bytes || bytes->empty()) {
        return nullptr;
    }

    return bytes;
}

define_reflectable_class_function(MFileManager, bytesFromFile)
MVector<uint8_t>::ptr MFileManager::bytesFromFile(const std::string &path) {
    auto bytes = MVector<uint8_t>::create();
    
    std::string nPath = m_normal_path path;
    dash::read_file(nPath.c_str(), [&](int size) {
        bytes->resize((size_t)size);
        return bytes->data();
    });

    return bytes->empty() ? nullptr : bytes;
}

define_reflectable_class_function(MFileManager, u8stringFromBundle, "args:path")
std::string MFileManager::u8stringFromBundle(const std::string &path) {
    if (path.empty()) {
        return "";
    }

    MVector<uint8_t>::ptr bytes = MBundle::instance()->loadResource(m_normal_path path);
    if (!bytes || bytes->empty()) {
        return "";
    }

    auto bgn = (char *)bytes->data();
    auto end = (char *)bytes->data() + bytes->size();
    return std::string(bgn, end);
}

define_reflectable_class_function(MFileManager, u8stringFromFile, "args:path")
std::string MFileManager::u8stringFromFile(const std::string &path) {
    std::string str;

    std::string nPath = m_normal_path path;
    dash::read_file(nPath.c_str(), [&](int size) {
        str.resize((size_t)(size + 1));
        return str.data();
    });

    return str;
}

define_reflectable_class_function(MFileManager, writeBytesToFile, "args:bytes,path")
void MFileManager::writeBytesToFile(const MVector<uint8_t>::ptr &bytes, const std::string &path) {
    std::string nPath = m_normal_path path;
    if (bytes) {
        dash::write_file(nPath.c_str(), bytes->data(), (int)bytes->size());
    } else {
        //clear the file.
        dash::write_file(nPath.c_str(), nullptr, 0);
    }
}

define_reflectable_class_function(MFileManager, writeU8StringToFile, "args:str,path")
void MFileManager::writeU8StringToFile(const std::string &string, const std::string &path) {
    std::string nPath = m_normal_path path;
    dash::write_file(nPath.c_str(), string.c_str(), (int)string.size());
}

define_reflectable_class_function(MFileManager, contentsOfDirectory, "args:path")
MVector<std::string>::ptr MFileManager::contentsOfDirectory(const std::string &path) {
    auto contents = MVector<std::string>::create();

    std::filesystem::directory_iterator iterator(m_normal_path path);
    for (auto &entry : iterator) {
        std::string item = entry.path().string();
        contents->push_back(m_normal_path item);
    }

    return contents;
}

define_reflectable_class_function(MFileManager, fileExistsAt, "args:path")
bool MFileManager::fileExistsAt(const std::string &path) {
    if (!path.empty()) {
        return std::filesystem::exists(m_normal_path path);
    }
    return false;
}

define_reflectable_class_function(MFileManager, directoryExistsAt, "args:path")
bool MFileManager::directoryExistsAt(const std::string &path) {
    if (!path.empty()) {
        return std::filesystem::is_directory(m_normal_path path);
    }
    return false;
}

define_reflectable_class_function(MFileManager, createDirectory, "args:path")
void MFileManager::createDirectory(const std::string &path) {
    if (!path.empty()) {
        std::filesystem::create_directories(m_normal_path path);
    }
}

define_reflectable_class_function(MFileManager, removePath, "args:path")
void MFileManager::removePath(const std::string &path) {
    if (!path.empty()) {
        std::filesystem::remove_all(m_normal_path path);
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

define_reflectable_class_function(MBundle, loadResource, "args:path")
MVector<uint8_t>::ptr MBundle::loadResource(const std::string &path) {
    if (!path.empty()) {
        return onLoadResource(path);
    }
    return nullptr;
}

define_reflectable_class_function(MBundle, resBundleDirectory, "getter")
define_reflectable_class_function(MBundle, documentDirectory , "getter")
define_reflectable_class_function(MBundle, temporaryDirectory, "getter")

#define _D(var, getter) if (var.empty()) { var = getter(); } return var;

std::string MBundle::resBundleDirectory() { _D(mResBundleDirectory, onGetResBundleDirectory) }
std::string MBundle::documentDirectory () { _D(mDocumentDirectory , onGetDocumentDirectory ) }
std::string MBundle::temporaryDirectory() { _D(mTemporaryDirectory, onGetTemporaryDirectory) }

#undef  _D

MVector<uint8_t>::ptr MBundle::onLoadResource(const std::string &path) {
    return nullptr;
}

std::string MBundle::onGetResBundleDirectory() { return ""; }
std::string MBundle::onGetDocumentDirectory () { return ""; }
std::string MBundle::onGetTemporaryDirectory() { return ""; }
