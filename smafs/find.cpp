// find.cpp : Defines the file_find_*_sma functions.
#include "pch.h"

namespace find
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

	HRESULT file_find_next_matching(bool advance = true)
	{
		for (;;)
		{
			if (advance)
			{
				if (!FindNextFileW(hFindFile, &ffd))
				{
					return HRESULT_FROM_WIN32(GetLastError());
				}
			}

			if ((ffd.dwFileAttributes & dwDesiredAttributes) != 0)
			{
				return smafs_success;
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

		DWORD dwAttr = dtoui32(attr);
		if (!SUCCEEDED(smafs_status))
		{
			return "";
		}

		if (hFindFile != INVALID_HANDLE_VALUE)
		{
			if (!FindClose(hFindFile))
			{
				// can be safely ignored
				GetLastError(); // to clear it
			}

			hFindFile = INVALID_HANDLE_VALUE;
		}

		dwDesiredAttributes = (dwAttr & allowed_filter_attributes)
			| FILE_ATTRIBUTE_NORMAL;

		// FindExInfoBasic doesn't populate ffd.cAlternateFileName
		// ...who's still using short 8.3 filenames?
		hFindFile = FindFirstFileExW(wmask, FindExInfoBasic, &ffd,
			FindExSearchNameMatch, nullptr, 0);

		HRESULT new_status = S_OK;
		if (hFindFile == INVALID_HANDLE_VALUE)
		{
			new_status = HRESULT_FROM_WIN32(GetLastError());
		}

		free(wmask);

		if (SUCCEEDED(new_status))
		{
			new_status = file_find_next_matching(false);

			if (new_status == smafs_no_more_files)
			{
				// return correct error status (FindFirstFileEx returns ERROR_FILE_NOT_FOUND if no files are found)
				new_status = smafs_file_not_found;
			}
		}

		smafs_status = new_status;

		return SUCCEEDED(new_status)
			? wcs2str_or_empty(ffd.cFileName)
			: "";
	}

	dllx const char* file_find_next_sma()
	{
		if (hFindFile == INVALID_HANDLE_VALUE)
		{
			smafs_status = smafs_invalid_operation;
			return "";
		}

		smafs_status = file_find_next_matching();
		return SUCCEEDED(smafs_status)
			? wcs2str_or_empty(ffd.cFileName)
			: "";
	}

	dllx double file_find_close_sma()
	{
		if (hFindFile == INVALID_HANDLE_VALUE)
		{
			smafs_status = FindClose(hFindFile)
				? smafs_success
				: HRESULT_FROM_WIN32(GetLastError());

			// clear the handle even if we didn't close it correctly
			//  (since failed closes can be ignored safely)
			hFindFile = INVALID_HANDLE_VALUE;
		}
		else
		{
			// doing nothing is a success in my books
			smafs_status = smafs_noop;
		}

		return SUCCEEDED(smafs_status);
	}
}
