// find.cpp : Defines the file_find_*_sma functions.
#include "pch.h"

HRESULT file_find_next_matching(bool advance = true)
{
	for (;;)
	{
		if (advance)
		{
			if (!FindNextFileW(gstate_find.hFindFile, &gstate_find.ffd))
			{
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		if ((gstate_find.ffd.dwFileAttributes & gstate_find.dwDesiredAttributes) != 0)
		{
			return S_OK;
		}

		advance = true;
	}
}

const uint32_t allowed_filter_attributes = FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN
	| FILE_ATTRIBUTE_SYSTEM
	| 8 // FAT_DIRENT_ATTR_VOLUME_ID, AKA fa_volumeid. not a valid attribute in NTFS, though
	| FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_ARCHIVE;

dllx const char* file_find_first_sma(const char* mask, double attr)
{
	wchar_t* wmask = str2wcs(mask);
	if (wmask == nullptr)
	{
		return "";
	}

	if (gstate_find.hFindFile != INVALID_HANDLE_VALUE)
	{
		if (!FindClose(gstate_find.hFindFile))
		{
			// can be safely ignored
			GetLastError(); // to clear it
		}
		
		gstate_find.hFindFile = INVALID_HANDLE_VALUE;
	}

	gstate_find.dwDesiredAttributes = (static_cast<uint32_t>(attr) & allowed_filter_attributes)
		| FILE_ATTRIBUTE_NORMAL;

	// FindExInfoBasic doesn't populate ffd.cAlternateFileName
	// ...who's still using short 8.3 filenames?
	gstate_find.hFindFile = FindFirstFileExW(wmask, FindExInfoBasic, &gstate_find.ffd,
		FindExSearchNameMatch, nullptr, 0);

	HRESULT new_state = S_OK;
	if (gstate_find.hFindFile == INVALID_HANDLE_VALUE)
	{
		new_state = HRESULT_FROM_WIN32(GetLastError());
	}

	free(wmask);

	if (SUCCEEDED(new_state))
	{
		new_state = file_find_next_matching(false);
	}

	return SUCCEEDED(new_state)
		? wcs2str_or_empty(gstate_find.ffd.cFileName)
		: "";
}

dllx const char* file_find_next_sma()
{
	if (gstate_find.hFindFile == INVALID_HANDLE_VALUE)
	{
		smafs_status = HRESULT_FROM_WIN32(ERROR_INVALID_OPERATION);
		return "";
	}

	smafs_status = file_find_next_matching();
	return SUCCEEDED(smafs_status)
		? wcs2str_or_empty(gstate_find.ffd.cFileName)
		: "";
}

dllx double file_find_close_sma()
{
	if (gstate_find.hFindFile == INVALID_HANDLE_VALUE)
	{
		smafs_status = FindClose(gstate_find.hFindFile)
			? S_OK
			: HRESULT_FROM_WIN32(GetLastError());

		// clear the handle even if we didn't close it correctly
		//  (since failed closes can be ignored safely)
		gstate_find.hFindFile = INVALID_HANDLE_VALUE;
	}
	else
	{
		// doing nothing is a success in my books
		smafs_status = S_FALSE;
	}

	return SUCCEEDED(smafs_status);
}
