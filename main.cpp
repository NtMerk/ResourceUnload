#include <Windows.h>
#include <stdio.h>
#include <fileapi.h>

#include "resource.h"

bool unloadResource(PCSTR resourceName)
{
	// Handles for the resource
	HRSRC hResource;
	HANDLE hResourceGlobal;

	// Handle and information about the output file
	HANDLE hFileHandle;
	char sFileName[64];
	LPCSTR pFilePtr;
	DWORD iFileSize;

	// Finding the resource TestResource with type TEXT - NULL indicates current module
	hResource = FindResource(NULL, MAKEINTRESOURCE(TestResource), L"TEXT");
	if (!hResource)
		return false;

	// Loading the resource - NULL indicates current module
	hResourceGlobal = LoadResource(NULL, hResource);
	if (!hResourceGlobal)
		return false;

	// Get size and pointer to the embedded file
	iFileSize = SizeofResource(NULL, hResource);
	pFilePtr = (LPCSTR)LockResource(hResourceGlobal);

	if (!pFilePtr)
		return false;

	// Choosing output name/directory
	sprintf(sFileName, "%s.txt", resourceName);

	// Opening handle to the output file
	hFileHandle = CreateFileA(sFileName, FILE_ALL_ACCESS, 0, NULL, CREATE_ALWAYS, 0, NULL);

	if (hFileHandle == INVALID_HANDLE_VALUE)
	{
		int err = GetLastError();
		if ((err == ERROR_ALREADY_EXISTS) || (err == ERROR_SHARING_VIOLATION))
		{
			// File exists or is locked
			return true;
		}

		printf("%s unload error %d\n", sFileName, err);
		return false;
	}

	// Writing file to disk byte by byte using the pointer to the embedded file
	while (iFileSize--)
	{
		unsigned long iNumWritten;
		WriteFile(hFileHandle, pFilePtr, 1, &iNumWritten, NULL);
		pFilePtr++;
	}

	CloseHandle(hFileHandle);
	
	return true;
}


int main()
{
	return unloadResource("ResourceOutput");
}