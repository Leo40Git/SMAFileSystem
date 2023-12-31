// smafs.h: Exposes common fields and functions.

#pragma once

// Debug output macro
#define trace(format, ...) { printf(format "\n", __VA_ARGS__); fflush(stdout); }

template<typename T>
inline T* malloc()
{
	return reinterpret_cast<T*>(malloc(sizeof(T)));
}

template<typename T>
inline T* calloc(size_t count)
{
	return reinterpret_cast<T*>(calloc(sizeof(T), count));
}

template<typename T>
inline T* realloc(T* block, size_t new_size)
{
	return reinterpret_cast<T*>(realloc(reinterpret_cast<void*>(block), new_size));
}

extern HRESULT smafs_status;

/// common status codes
/// keep these in sync with the #macros in smafs.gml
/// (manually, because GmxGen can't expand defines and doesn't see these header files anyway)
/// TODO: automate this?

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

/// Special functions for interoperation with GameMaker Language.
namespace gml
{
	// see https://web.archive.org/web/20160303070839/https://help.yoyogames.com/hc/en-us/articles/216755258-Returning-Values-From-An-Extension-Asynchronously-GMS-v1-3-

	typedef int32_t ds_map_id;
	typedef void(*fp_event_perform_async)(ds_map_id map, int32_t event_type);
	typedef ds_map_id(*fp_ds_map_create_ext)(int32_t n, ...);
	typedef bool(*fp_ds_map_set_real)(ds_map_id map, const char* key, double value);
	typedef bool(*fp_ds_map_set_string)(ds_map_id map, const char* key, const char* value);

	extern fp_event_perform_async event_perform_async;
	extern fp_ds_map_create_ext ds_map_create_ext;
	extern fp_ds_map_set_real ds_map_set_real;
	extern fp_ds_map_set_string ds_map_set_string;

	inline ds_map_id ds_map_create()
	{
		return ds_map_create_ext(0);
	}

	const int32_t EVENT_OTHER_SOCIAL = 70;
	const int32_t EVENT_OTHER_ASYNC_SAVE_LOAD = 72;
}

/// Global state for functions in "find.cpp".
namespace find
{
	void init();
}
