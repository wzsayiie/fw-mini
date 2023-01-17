#pragma once

#include "mdata.h"
#include "mgeneric.h"

m_class(MFileManager, MObject) {
public:
    static MFileManager *instance();

public:
    //data read and write:
    MData::ptr bytesFromBundle(const std::string &path);
    MData::ptr bytesFromFile  (const std::string &path);

    std::string u8stringFromBundle(const std::string &path);
    std::string u8stringFromFile  (const std::string &path);

    void writeBytesToFile   (const MData::ptr  &data  , const std::string &path);
    void writeU8StringToFile(const std::string &string, const std::string &path);

    //path control:
    MVector<std::string>::ptr contentsOfDirectory(const std::string &path);

    bool fileExistsAt     (const std::string &path); //directory also recognized.
    bool directoryExistsAt(const std::string &path);
    void createDirectory  (const std::string &path);
    void removePath       (const std::string &path);
};
