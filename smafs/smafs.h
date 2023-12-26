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

wchar_t* str2wcs(const char* src, uint32_t codepage = CP_UTF8);

char* wcs2str(const wchar_t* src, uint32_t codepage = CP_UTF8);

inline const char* wcs2str_or_empty(wchar_t* src, uint32_t codepage = CP_UTF8)
{
	const char* dest = wcs2str(src, codepage);
	return dest != nullptr ? dest : "";
}

/// Global state for functions in "find.cpp".
namespace find
{
	extern HANDLE hFindFile;
	extern WIN32_FIND_DATAW ffd;
	extern uint32_t dwDesiredAttributes;
}
