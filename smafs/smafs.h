#pragma once
#include "pch.h"

extern double smafs_status;
dllx double smafs_get_status();

void* malloc(size_t size);

template<typename T>
inline T* malloc()
{
	return (T*)malloc(sizeof(T));
}

inline void* calloc(size_t size, size_t count)
{
	return malloc(count * size);
}

template<typename T>
inline T* calloc(size_t count)
{
	return (T*)calloc(sizeof(T), count);
}

void* realloc(void* block, size_t size);

inline void* recalloc(void* block, size_t size, size_t count)
{
	return realloc(block, count * size);
}

template<typename T>
inline T* recalloc(T* block, size_t count)
{
	return (T*)recalloc(block, sizeof(T), count);
}

void free(void* block);

inline wchar_t* str2wcs(const char* src, UINT codepage = CP_UTF8)
{
	size_t size = MultiByteToWideChar(codepage, 0, src, -1, nullptr, 0);
	if (size == 0)
	{
		smafs_status = GetLastError();
		return nullptr;
	}

	wchar_t* dest = calloc<wchar_t>(size);
	if (dest != nullptr)
	{
		if (MultiByteToWideChar(codepage, 0, src, -1, dest, size) != 0)
		{
			smafs_status = 0;
		}
		else
		{
			smafs_status = GetLastError();
			free(dest);
			dest = nullptr;
		}
	}

	return dest;
}

inline char* wcs2str(const wchar_t* src, UINT codepage = CP_UTF8)
{
	size_t size = WideCharToMultiByte(codepage, 0, src, -1, nullptr, 0, nullptr, nullptr);
	if (size == 0)
	{
		smafs_status = GetLastError();
		return nullptr;
	}

	char* dest = calloc<char>(size);
	if (dest != nullptr)
	{
		if (WideCharToMultiByte(codepage, 0, src, -1, dest, size, nullptr, nullptr) != 0)
		{
			smafs_status = 0;
		}
		else
		{
			smafs_status = GetLastError();
			free(dest);
			dest = nullptr;
		}
	}

	return dest;
}

inline const char* wcs2str_notnull(const wchar_t* src, UINT codepage = CP_UTF8)
{
	const char* dest = wcs2str(src, codepage);
	if (dest != nullptr)
	{
		return dest;
	}
	else
	{
		return "";
	}
}

extern HANDLE hFindFile;
dllx const char* file_find_first_sma(const char* mask, double attr);
dllx const char* file_find_next_sma();
dllx double file_find_close_sma();
