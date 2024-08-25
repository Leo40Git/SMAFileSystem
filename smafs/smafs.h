// smafs.h: Exposes common fields and functions.

#pragma once

// Debug output macro
#define trace(format, ...) { printf(format "\n", __VA_ARGS__); fflush(stdout); }

/*
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
*/

extern HRESULT smafs_status;

/// common status codes
/// keep these in sync with the #macros in smafs.gml
/// (manually, because GmxGen can't expand defines and doesn't see these header files anyway)
/// TODO: automate this?

#define smafs_success				S_OK
#define smafs_noop					S_FALSE
#define smafs_file_not_found		__HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)
#define smafs_path_not_found		__HRESULT_FROM_WIN32(ERROR_PATH_NOT_FOUND)
#define smafs_too_many_open_files	__HRESULT_FROM_WIN32(ERROR_TOO_MANY_OPEN_FILES)
#define smafs_access_denied			__HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED)
#define smafs_out_of_memory			__HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY)
#define smafs_no_more_files			__HRESULT_FROM_WIN32(ERROR_NO_MORE_FILES)
#define smafs_invalid_argument		__HRESULT_FROM_WIN32(ERROR_INVALID_PARAMETER)
#define smafs_invalid_operation		__HRESULT_FROM_WIN32(ERROR_INVALID_OPERATION)

namespace smafs
{
	LPTSTR A2T(LPCSTR src);

	LPSTR T2A(LPCTSTR src);

	inline LPCSTR T2A_NONNULL(LPCTSTR src)
	{
		LPSTR dest = T2A(src);
		return dest != nullptr ? dest : "";
	}

	int32_t dtoi32(double in);

	uint32_t dtoui32(double in);
}

/// Initializes state local to find.cpp
void smafs_find_init();
