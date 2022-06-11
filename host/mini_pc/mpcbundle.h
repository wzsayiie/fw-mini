#pragma once

#include "mfilemanager.h"

minikit_class(MPCBundle, MBundle) {
public:
    static void install();

protected:
    MVector<uint8_t>::ptr onLoadResource(const std::string &path) override;

    std::string onGetResBundleDirectory() override;
    std::string onGetDocumentDirectory () override;
    std::string onGetTemporaryDirectory() override;
};
