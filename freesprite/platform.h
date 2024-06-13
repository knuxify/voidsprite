#pragma once

#if _WIN32
#define PlatformNativePathString std::wstring
#define PlatformFileModeRB L"rb"
#define PlatformFileModeWB L"wb"
#else
#define PlatformNativePathString std::string
#define PlatformFileModeRB "rb"
#define PlatformFileModeWB "wb"
#endif

void platformPreInit();
void platformInit();
void platformPostInit();

void platformTrySaveImageFile(EventCallbackListener* caller);
void platformTryLoadImageFile(EventCallbackListener* caller);
void platformOpenFileLocation(PlatformNativePathString path);

FILE* platformOpenFile(PlatformNativePathString path, PlatformNativePathString mode);