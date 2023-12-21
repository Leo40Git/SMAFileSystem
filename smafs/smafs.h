// smafs.h: Exposes common fields and functions.

#pragma once

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
dllx double smafs_get_status();

wchar_t* str2wcs(const char* src, uint32_t codepage = CP_UTF8);

char* wcs2str(const wchar_t* src, uint32_t codepage = CP_UTF8);

inline const char* wcs2str_or_empty(wchar_t* src, uint32_t codepage = CP_UTF8)
{
	const char* dest = wcs2str(src, codepage);
	return dest != nullptr ? dest : "";
}

// global state for functions in find.cpp
extern struct global_state_find_t
{
	HANDLE hFindFile;
	WIN32_FIND_DATAW ffd;
	uint32_t dwDesiredAttributes;

	void init()
	{
		hFindFile = INVALID_HANDLE_VALUE;
		memset(&ffd, 0, sizeof(ffd));
		dwDesiredAttributes = FILE_ATTRIBUTE_NORMAL;
	}
} gstate_find;
