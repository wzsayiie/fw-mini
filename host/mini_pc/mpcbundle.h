#pragma once

#include "mfilemanager.h"

declare_reflectable_class(MPCBundle)
class MPCBundle : public MExtends<MPCBundle, MBundle> {
public:
    static void install();

public:
    MVector<uint8_t>::ptr loadAsset(const std::string &path) override;

    std::string documentDirectory () override;
    std::string temporaryDirectory() override;

private:
    std::filesystem::path mBundeDirectory;
};
