#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "WinGUI.h"

_WIN_GUI_API_ _Bool WinGUI_GetVer(
	WinGUI_Ver *self
) {
	
	if (self == NULL) {
		return 0;
	}

	HMODULE hModule = NULL;

	GetModuleHandleEx(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
			GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPCTSTR)(WinGUI_GetVer), &hModule
	);

	TCHAR ModuleName[MAX_PATH + 1] = { 0 };
	GetModuleFileName(hModule, ModuleName, MAX_PATH + 1);

	DWORD VSVerSize = GetFileVersionInfoSize(ModuleName, NULL);

	if (VSVerSize == 0) {
		return 0;
	}

	LPBYTE VSVer = (LPBYTE)(LocalAlloc(LPTR, VSVerSize));

	if (VSVer == NULL) {
		return 0;
	}
	
	GetFileVersionInfo(ModuleName, NULL, VSVerSize, VSVer);

	UINT unused = 0;
	VS_FIXEDFILEINFO *VSVerFixed = NULL;
	
	VerQueryValue(VSVer, TEXT("\\"), &VSVerFixed, &unused);

	self->version =
		VSVerFixed->dwProductVersionMS;

	self->major = VSVerFixed->dwProductVersionMS >> 16;
	self->minor = VSVerFixed->dwProductVersionMS;

	LocalFree((HLOCAL)(VSVer));

	return 1;

}
