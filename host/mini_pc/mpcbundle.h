#pragma once

#include "mbundle.h"

m_class(MPCBundle, MBundle) {
public:
    static void install();

protected:
    MData::ptr onLoadResource(const std::string &path) override;

    std::string onGetResBundleDirectory() override;
    std::string onGetDocumentDirectory () override;
    std::string onGetTemporaryDirectory() override;
};
