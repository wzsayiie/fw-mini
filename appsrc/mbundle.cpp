#include "mbundle.h"
#include "rdefine.h"

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
