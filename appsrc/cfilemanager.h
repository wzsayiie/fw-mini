#pragma once

#include "minikit.h"

//------------------------------------------------------------------------------
//path:

void CAppendPathItem(std::string *path, const std::string &item);

std::string CGetPathFilename(const std::string &path);
std::string CGetPathParent  (const std::string &path);

//------------------------------------------------------------------------------
//file operations:

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

bool CMakeDirectory(const std::string &path);
std::vector<std::string> CDirectorySubItems(const std::string &path);

void CRemovePath(const std::string &path);
bool CPathExists(const std::string &path, bool *isDirectory);
