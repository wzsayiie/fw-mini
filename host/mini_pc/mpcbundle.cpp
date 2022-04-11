#include "mpcbundle.h"

void MPCBundle::install() {
    auto obj = MPCBundle::create();
    setSharedObject(obj);
}

MVector<uint8_t>::ptr MPCBundle::loadAsset(const std::string &path) {
    return nullptr;
}

std::string MPCBundle::documentDirectory() {
    return "";
}

std::string MPCBundle::temporaryDirectory() {
    return "";
}
