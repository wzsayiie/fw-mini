#pragma once

#include "mfilemanager.h"

m_class(MPCBundle, MBundle) {
public:
    static void install();

public:
    MVector<uint8_t>::ptr loadAsset(const std::string &path) override;

    std::string documentDirectory () override;
    std::string temporaryDirectory() override;

private:
    std::filesystem::path mBundeDirectory;
};
