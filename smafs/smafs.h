// smafs.h: Exposes common fields and functions.

#pragma once

// Debug output macro
#define trace(format, ...) { printf(format "\n", __VA_ARGS__); fflush(stdout); }

template<typename T>
inline T* malloc()
{
	return static_cast<T*>(std::malloc(sizeof(T)));
}

template<typename T>
inline T* calloc(size_t count)
{
	return static_cast<T*>(std::calloc(sizeof(T), count));
}

template<typename T>
inline T* realloc(T* block, size_t new_size)
{
	return static_cast<T*>(std::realloc(block, new_size));
}

extern HRESULT smafs_status;

/// common status codes
/// keep these in sync with the extension macros

#define smafs_success				S_OK
#define smafs_noop					S_FALSE
#define smafs_file_not_found		HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)
#define smafs_path_not_found		HRESULT_FROM_WIN32(ERROR_PATH_NOT_FOUND)
#define smafs_too_many_open_files	HRESULT_FROM_WIN32(ERROR_TOO_MANY_OPEN_FILES)
#define smafs_access_denied			HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED)
#define smafs_out_of_memory			HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY)
#define smafs_no_more_files			HRESULT_FROM_WIN32(ERROR_NO_MORE_FILES)
#define smafs_invalid_argument		HRESULT_FROM_WIN32(ERROR_INVALID_PARAMETER)
#define smafs_invalid_operation		HRESULT_FROM_WIN32(ERROR_INVALID_OPERATION)

wchar_t* str2wcs(const char* src, uint32_t codepage = CP_UTF8);

char* wcs2str(const wchar_t* src, uint32_t codepage = CP_UTF8);

inline const char* wcs2str_or_empty(wchar_t* src, uint32_t codepage = CP_UTF8)
{
	const char* dest = wcs2str(src, codepage);
	return dest != nullptr ? dest : "";
}

uint32_t dtoui32(double in);

/// Global state for functions in "find.cpp".
namespace find
{
	void init();
}
