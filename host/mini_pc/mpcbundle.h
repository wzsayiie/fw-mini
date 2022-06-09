#pragma once

#include "mfilemanager.h"

m_class(MPCBundle, MBundle) {
public:
    static void install();

protected:
    MVector<uint8_t>::ptr onLoadAsset(const std::string &path) override;

    std::string onGetBundleDirectory   () override;
    std::string onGetDocumentDirectory () override;
    std::string onGetTemporaryDirectory() override;
};
