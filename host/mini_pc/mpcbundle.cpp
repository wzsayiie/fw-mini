#include "mpcbundle.h"

#if DASH_OS_WIN32
    #include <ShlObj.h>
    #include <Windows.h>
#elif DASH_OS_LINUX
    #include <pwd.h>
    #include <unistd.h>
#endif

void MPCBundle::install() {
    auto obj = MPCBundle::create();
    setSharedObject(obj);
}

MVector<uint8_t>::ptr MPCBundle::loadAsset(const std::string &path) {
    if (mBundeDirectory.empty()) {
    #if DASH_OS_WIN32
        GetModuleFileNameW(nullptr, dash::buffer<WCHAR>(), (DWORD)dash::buffer_size<WCHAR>());
        std::filesystem::path exePath = dash::buffer<char16_t>();

    #elif DASH_OS_LINUX
        readlink("/proc/self/exe", dash::buffer<char>(), (size_t)dash::buffer_size<char>());
        std::filesystem::path exePath = dash::buffer<char>();
    #endif

        for (auto &it : exePath.parent_path()) {
            mBundeDirectory.append(it.string());

            //NOTE: if the executable is in the project, use the resources from the project.
            if (it == "fw-mini") {
                mBundeDirectory.append("appres");
                break;
            }
        }
        mBundeDirectory.append(MBundle::BundleDirectoryName);
    }

    std::filesystem::path assetPath = mBundeDirectory;
    assetPath.append(path);

    auto asset = MVector<uint8_t>::create();
    dash::read_file(assetPath.string().c_str(), [&](int size) {
        asset->vector.resize((size_t)size);
        return asset->vector.data();
    });
    return asset;
}

std::string MPCBundle::documentDirectory() {
#if DASH_OS_WIN32
    SHGetFolderPathW(nullptr, CSIDL_PERSONAL, nullptr, 0, dash::buffer<WCHAR>());
    std::filesystem::path directory = dash::buffer<char16_t>();

#elif DASH_OS_LINUX
    passwd *pw = getpwuid(getuid());
    std::filesystem::path directory = pw->pw_dir;
#endif

    directory.append(MBundle::PrivateDirectoryName);
    std::filesystem::create_directory(directory);

    return directory.string();
}

std::string MPCBundle::temporaryDirectory() {
#if DASH_OS_WIN32
    GetTempPathW((DWORD)dash::buffer_size<WCHAR>(), dash::buffer<WCHAR>());
    std::wstring shortPath = dash::buffer<WCHAR>();

    GetLongPathNameW(shortPath.c_str(), dash::buffer<WCHAR>(), (DWORD)dash::buffer_size<WCHAR>());
    std::filesystem::path directory = dash::buffer<char16_t>();

#elif DASH_OS_LINUX
    std::filesystem::path directory = std::filesystem::temp_directory_path();
#endif

    directory.append(MBundle::PrivateDirectoryName);
    std::filesystem::create_directory(directory);

    return directory.string();
}
