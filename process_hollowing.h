#pragma once
#include "windows.h"
#include "stdio.h"



BOOL loadNtUnmapViewOfSection();
BOOL processHollowing(LPWSTR targetPath, PVOID pBuffer, DWORD dwBufferSize);