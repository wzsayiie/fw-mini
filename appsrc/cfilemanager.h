#pragma once

#include <string>
#include <vector>

//------------------------------------------------------------------------------
//path join:

void CAppendPathItem(std::string *path, const std::string &item);

std::string CGetPathFilename(const std::string &path);
std::string CGetPathParent  (const std::string &path);

//------------------------------------------------------------------------------
//file operations:

std::vector<uint8_t> CReadFile        (const std::string &path);
std::string          CReadTextFile    (const std::string &path);
std::vector<uint8_t> CReadBundleAsset (const std::string &path);
std::string          CReadBundleString(const std::string &path);

void CWriteFile   (const std::string &path, const std::vector<uint8_t> &data);
void CWriteU8File (const std::string &path, const std::string &data);
void CWriteU16File(const std::string &path, const std::string &data);

std::string CDocumentPath ();
std::string CCachePath    ();
std::string CTemporaryPath();

bool CMakeDirectory(const std::string &path);
std::vector<std::string> CDirectorySubItems(const std::string &path);

void CRemovePath(const std::string &path);
bool CPathExists(const std::string &path, bool *isDirectory);
