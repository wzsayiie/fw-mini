#pragma once

#include "minikit.h"

std::vector<uint8_t> CReadDataFromFile    (const std::string &path);
std::string          CReadStringFromFile  (const std::string &path);
std::vector<uint8_t> CReadDataFromBundle  (const std::string &path);
std::string          CReadStringFromBundle(const std::string &path);

void CWriteDataToFile     (const std::string &path, const std::vector<uint8_t> &data);
void CWriteU8StringToFile (const std::string &path, const std::string &data);
void CWriteU16StringToFile(const std::string &path, const std::string &data);

std::string CDocumentPath ();
std::string CCachePath    ();
std::string CTemporaryPath();
