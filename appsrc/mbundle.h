#pragma once

#include "mdata.h"

m_class(MBundle, MObject) {
public:
    M_HOST_CALL static void setInstance(const MBundle::ptr &obj);
    static MBundle *instance();

public:
    std::string getDocumentFilePath (const std::string &fileName);
    std::string getTemporaryFilePath(const std::string &fileName);

    MData::ptr loadResource(const std::string &path);

    std::string resBundleDirectory();
    std::string documentDirectory ();
    std::string temporaryDirectory();

protected:
    M_HOST_IMPL virtual MData::ptr onLoadResource(const std::string &path);

    M_HOST_IMPL virtual std::string onGetResBundleDirectory();
    M_HOST_IMPL virtual std::string onGetDocumentDirectory ();
    M_HOST_IMPL virtual std::string onGetTemporaryDirectory();

private:
    static MBundle::ptr sInstance;

private:
    std::string mResBundleDirectory;
    std::string mDocumentDirectory ;
    std::string mTemporaryDirectory;
};
