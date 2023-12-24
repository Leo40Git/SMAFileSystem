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
dllx double smafs_get_status();

namespace gml
{
	typedef int32_t ds_map;
	typedef void(*event_perform_async_t)(ds_map map, int32_t event_type);
	typedef ds_map(*ds_map_create_ext_t)(int32_t n, ...);
	typedef bool(*ds_map_set_double_t)(ds_map map, const char* key, double value);
	typedef bool(*ds_map_set_string_t)(ds_map map, const char* key, const char* value);

	// see https://web.archive.org/web/20160303070839/https://help.yoyogames.com/hc/en-us/articles/216755258-Returning-Values-From-An-Extension-Asynchronously-GMS-v1-3-

	extern event_perform_async_t event_perform_async;
	extern ds_map_create_ext_t ds_map_create_ext;
	extern ds_map_set_double_t ds_map_set_double;
	extern ds_map_set_string_t ds_map_set_string;

	inline ds_map ds_map_create()
	{
		return ds_map_create_ext(0);
	}

	const int32_t EVENT_OTHER_SOCIAL = 70;
	const int32_t EVENT_OTHER_ASYNC_SAVE_LOAD = 72;
}

wchar_t* str2wcs(const char* src, uint32_t codepage = CP_UTF8);

char* wcs2str(const wchar_t* src, uint32_t codepage = CP_UTF8);

inline const char* wcs2str_or_empty(wchar_t* src, uint32_t codepage = CP_UTF8)
{
	const char* dest = wcs2str(src, codepage);
	return dest != nullptr ? dest : "";
}

// global state for functions in find.cpp
namespace find
{
	extern HANDLE hFindFile;
	extern WIN32_FIND_DATAW ffd;
	extern uint32_t dwDesiredAttributes;
}
