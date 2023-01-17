#include "mpcbundle.h"
#include <filesystem>
#include "MPROFILE.h"
#include "dbuffer.h"
#include "dfile.h"

#if D_OS_WIN32
    #include <ShlObj.h>
    #include <Windows.h>
#elif D_OS_LINUX
    #include <pwd.h>
    #include <unistd.h>
#endif

void MPCBundle::install() {
    auto obj = MPCBundle::create();
    setInstance(obj);
}

MData::ptr MPCBundle::onLoadResource(const std::string &path) {
    std::filesystem::path resPath = resBundleDirectory();
    resPath.append(path);

    auto res = MData::create();
    dash::read_file(resPath.string().c_str(), [&](int size) {
        res->resize(size);
        return res->bytes();
    });
    return res;
}

std::string MPCBundle::onGetResBundleDirectory() {
#if D_OS_WIN32
    GetModuleFileNameW(nullptr, dash::buffer<WCHAR>(), (DWORD)dash::buffer_size<WCHAR>());
    std::filesystem::path exePath = dash::buffer<char16_t>();

#elif D_OS_LINUX
    readlink("/proc/self/exe", dash::buffer<char>(), (size_t)dash::buffer_size<char>());
    std::filesystem::path exePath = dash::buffer<char>();
#endif

    std::filesystem::path directory;
    for (auto &it : exePath.parent_path()) {
        directory.append(it.string());

        //NOTE: if the executable is in the project, use the resources from the project.
        if (it == "fw-mini") {
            directory.append("appres");
            break;
        }
    }
    directory.append(mp_resbundle_folder_name);

    return directory.string();
}

std::string MPCBundle::onGetDocumentDirectory() {
#if D_OS_WIN32
    SHGetFolderPathW(nullptr, CSIDL_PERSONAL, nullptr, 0, dash::buffer<WCHAR>());
    std::filesystem::path directory = dash::buffer<char16_t>();

#elif D_OS_LINUX
    passwd *pw = getpwuid(getuid());
    std::filesystem::path directory = pw->pw_dir;
#endif

    directory.append(mp_document_folder_name);
    std::filesystem::create_directory(directory);

    return directory.string();
}

std::string MPCBundle::onGetTemporaryDirectory() {
#if D_OS_WIN32
    GetTempPathW((DWORD)dash::buffer_size<WCHAR>(), dash::buffer<WCHAR>());
    std::wstring shortPath = dash::buffer<WCHAR>();

    GetLongPathNameW(shortPath.c_str(), dash::buffer<WCHAR>(), (DWORD)dash::buffer_size<WCHAR>());
    std::filesystem::path directory = dash::buffer<char16_t>();

#elif D_OS_LINUX
    std::filesystem::path directory = std::filesystem::temp_directory_path();
#endif

    directory.append(mp_temporary_folder_name);
    std::filesystem::create_directory(directory);

    return directory.string();
}
