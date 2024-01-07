#pragma comment(linker,"\
	\"/manifestdependency:type='win32' \
	name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
	processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"\
")

#include <stdint.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <winioctl.h>
#include <Dbt.h>

#include "..\Resources\Resource.h"

#include <WinGUI.h>

#if defined(_UNICODE) || defined(UNICODE)
	#define _WinMain wWinMain
#else // ASCII
	#define _WinMain WinMain
#endif

LRESULT CALLBACK WndProc(
	_In_ HWND hwnd,
	_In_ UINT uMsg,
	_In_opt_ WPARAM wParam,
	_In_opt_ LPARAM lParam
);

typedef struct _Main_Window {
	int initCX;
	int initCY;
	TCHAR *outputText;
	HFONT main;
	HFONT paragraph;
	COLORREF textColor;
	hRand random;
	HDEVNOTIFY hDevNot;
	HWND hDevCombo;
	WinGUI_Ver WinGUIVer;
	int exitCode;
} Main_Window;

int WINAPI _WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR lpCmdLine,
	_In_ int nCmdShow
) {

	WNDCLASSEX wcex = { 0 };

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	
	wcex.hIcon = LoadImage(hInstance, MAKEINTRESOURCE(IDI_APP),
		IMAGE_ICON, 0, 0, LR_VGACOLOR);

	if (wcex.hIcon == NULL) {
		return (int)(GetLastError());
	}

	wcex.hIconSm = wcex.hIcon;

	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);

	if (wcex.hCursor == NULL) {
		return (int)(GetLastError());
	}

	wcex.lpszClassName = TEXT("SandBox.Main_Window");

	// ========================================
	// Register "Main_Window" Class

	if (RegisterClassEx(&wcex) == (WORD)(0)) {
		return (int)(GetLastError());
	}

	// ========================================

	SIZE screen = { 0 };
	screen.cx = GetSystemMetrics(SM_CXSCREEN);
	screen.cy = GetSystemMetrics(SM_CYSCREEN);

	const SIZE initial = {
		.cx = 720,
		.cy = 480
	};

	// ========================================
	// Create Main Window

	HWND hwnd = CreateWindowEx(
		(DWORD)(0),
		wcex.lpszClassName,
		TEXT("SandBox"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		screen.cx / 2 - initial.cx / 2,
		screen.cy / 2 - initial.cy / 2,
		initial.cx, initial.cy,
		HWND_DESKTOP,
		NULL,
		hInstance,
		NULL
	);

	if (hwnd == NULL) {
		return (int)(GetLastError());
	}

	ShowWindow(hwnd, nCmdShow);

	// ========================================
	// Main Message Loop

	MSG msg = { 0 };
	
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// ========================================

	DestroyIcon(wcex.hIcon);
	DestroyCursor(wcex.hCursor);

	return (int)(msg.wParam);

}

LRESULT WndProc(
	_In_ HWND hwnd,
	_In_ UINT uMsg,
	_In_opt_ WPARAM wParam,
	_In_opt_ LPARAM lParam
) {
	
	switch (uMsg) {

		case WM_CREATE: {

			Main_Window *userData =
				(Main_Window*)(LocalAlloc(LPTR, sizeof(Main_Window)));

			if (userData == NULL) {
				return -1;
			}

			userData->initCX = ((LPCREATESTRUCT)(lParam))->cx;
			userData->initCY = ((LPCREATESTRUCT)(lParam))->cy;
			userData->outputText = NULL;
			userData->exitCode = 0;
			userData->main = CreateFont(48, 0, 0, 0, FW_MEDIUM,
				FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, OUT_DEFAULT_PRECIS,
				ANTIALIASED_QUALITY, VARIABLE_PITCH, TEXT("Calibri"));

			if (userData->main == NULL) {
				return -1;
			}

			SYSTEMTIME st = { 0 };
			GetLocalTime(&st);

			userData->random = Rand_Create((st.wHour * 120 + st.wMinute * 60 +
				st.wSecond) * 1000 + st.wMilliseconds);

			if (userData->random == NULL) {
				return -1;
			}

			userData->textColor = RGB(255, 255, 255);

			HWND hComboBox = CreateWindowEx(NULL, TEXT("ComboBox"), TEXT(""),
				WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 10, 10,
				((LPCREATESTRUCT)(lParam))->cx / 4,
				28, hwnd, (HMENU)(0x01), ((LPCREATESTRUCT)(lParam))->hInstance, NULL);

			if (hComboBox == NULL) {
				return -1;
			}

			HFONT hParagraph = CreateFont(24, 0, 0, 0, FW_NORMAL,
				FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, OUT_DEFAULT_PRECIS,
				ANTIALIASED_QUALITY, VARIABLE_PITCH, TEXT("Consolas"));

			if (hParagraph == NULL) {
				return -1;
			}

			userData->paragraph = hParagraph;

			SendMessage(hComboBox, WM_SETFONT, (WPARAM)(userData->paragraph), (LPARAM)(1));

			userData->hDevCombo = hComboBox;

			if (!WinGUI_GetVer(&userData->WinGUIVer)) {
				return -1;
			}

			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)(userData));

			DEV_BROADCAST_VOLUME devFilter = { 0 };
			devFilter.dbcv_size = sizeof(devFilter);
			devFilter.dbcv_devicetype = DBT_DEVTYP_VOLUME;
			devFilter.dbcv_unitmask = 0xFFFFFFFF;
			devFilter.dbcv_flags = DBTF_MEDIA;

			userData->hDevNot = RegisterDeviceNotification(hwnd, &devFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
			
			SendMessage(hwnd, WM_DEVICECHANGE,
				(WPARAM)(DBT_CONFIGCHANGED), (LPARAM)(0));

			if (SetTimer(hwnd, 0xFF, 1000, NULL) == 0) {
				return -1;
			}

			PostMessage(hwnd, WM_TIMER, 0xFF, NULL);

			return 0;
		}
		
		case WM_DEVICECHANGE: {

			Main_Window *userData =
				(Main_Window*)(GetWindowLongPtr(hwnd, GWLP_USERDATA));

			SendMessage(userData->hDevCombo, CB_RESETCONTENT,
				(WPARAM)(0), (LPARAM)(0));

			TCHAR drive[7];
			Str_Cpy(drive, 7, TEXT("\\\\.\\C:"));

			TCHAR outDrive[156] = { 0 };

			DWORD drives = GetLogicalDrives();
			for (size_t i = 2; i < sizeof(drives) * 8; i++) {
				if ((drives & (0x00000001 << i)) != 0) {

					HANDLE hDrive = CreateFile(
						drive, NULL, 0,
						NULL, OPEN_EXISTING,
						NULL, NULL
					);

					if (hDrive != INVALID_HANDLE_VALUE) {

						DWORD unused = 0;
						STORAGE_DEVICE_NUMBER device = { 0 };

						DeviceIoControl(
							hDrive,
							IOCTL_STORAGE_GET_DEVICE_NUMBER,
							NULL, 0,
							&device,
							sizeof(device),
							&unused,
							NULL
						);


						if (device.DeviceType != FILE_DEVICE_DISK) {
							drive[4]++;
							continue;
						}

						Str_Cat(outDrive, 156, drive + 4);
						Str_Cat(outDrive, 156, TEXT(" (hd"));
						
						Str_i32toStr(device.DeviceNumber, 1,
							outDrive + Str_Len(outDrive), 156 - Str_Len(outDrive));

						Str_Cat(outDrive, 156, TEXT(","));

						Str_i32toStr(device.PartitionNumber, 1,
							outDrive + Str_Len(outDrive), 156 - Str_Len(outDrive));

						Str_Cat(outDrive, 156, TEXT(")"));

						CloseHandle(hDrive);
						
						SendMessage(userData->hDevCombo, CB_ADDSTRING, (WPARAM)(0), (LPARAM)(outDrive));

						Str_Cpy(outDrive, 156, TEXT(""));

					}

				}

				drive[4]++;

			}

			if (drives != 0) {
				SendMessage(userData->hDevCombo, CB_SETCURSEL, (WPARAM)(0), (LPARAM)(NULL));
			}

			return 1;

		}
		
		case WM_ERASEBKGND: {
			return TRUE;
		}
		
		case WM_TIMER: {

			if (wParam == 0xFF) {

				Main_Window *userData =
					(Main_Window*)(GetWindowLongPtr(hwnd, GWLP_USERDATA));

				SYSTEMTIME st = { 0 };
				GetLocalTime(&st);

				int size = GetTimeFormat(LOCALE_SYSTEM_DEFAULT, 0,
					&st, NULL, NULL, 0);

				TCHAR *temp = LocalAlloc(LPTR, sizeof(TCHAR) * (size + 1));
				if (temp == NULL) {
					KillTimer(hwnd, 0xFF);
					userData->exitCode = 1;
					DestroyWindow(hwnd);
				}

				LocalFree(userData->outputText);

				userData->outputText = temp;

				GetTimeFormat(LOCALE_SYSTEM_DEFAULT, 0,
					&st, NULL, userData->outputText, size);

				userData->textColor = RGB(
					(Rand_Next(userData->random) % 66) + 190,
					(Rand_Next(userData->random) % 66) + 190,
					(Rand_Next(userData->random) % 66) + 190
				);

				RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT);

			}

			return 0;

		}
		
		case WM_PAINT: {

			Main_Window *userData =
				(Main_Window*)(GetWindowLongPtr(hwnd, GWLP_USERDATA));

			PAINTSTRUCT ps = { 0 };

			HDC hdc = BeginPaint(hwnd, &ps);

			RECT client = { 0 };
			GetClientRect(hwnd, &client);

			HDC MemDC = CreateCompatibleDC(hdc);
			HBITMAP MemBitmap = CreateCompatibleBitmap(hdc,
				client.right - client.left, client.bottom - client.top);
			
			HGDIOBJ oldMemBitmap = SelectObject(MemDC, MemBitmap);

			FillRect(MemDC, &client, GetStockObject(BLACK_BRUSH));

			if (userData->outputText != NULL) {

				int prevBKMode = SetBkMode(MemDC, TRANSPARENT);
				COLORREF prevColor = SetTextColor(MemDC, userData->textColor);
				HFONT prevFont = SelectObject(MemDC, userData->main);

				SIZE textExtent = { 0 };
				GetTextExtentPoint(MemDC, userData->outputText,
					(int)(Str_Len(userData->outputText)), &textExtent);

				TextOut(
					MemDC,
					(client.right - client.left) / 2 - textExtent.cx / 2,
					(client.bottom - client.top) / 2 - textExtent.cy / 2,
					userData->outputText,
					(int)(Str_Len(userData->outputText))
				);

				SelectObject(MemDC, userData->paragraph);

				TCHAR version[41] = { 0 };

				Str_Cpy(version, 41, TEXT("WinGUI: "));
				Str_i32toStr(userData->WinGUIVer.major, 1,
					version + Str_Len(version), 41 - Str_Len(version));
				Str_Cpy(version + Str_Len(version),
					41 - Str_Len(version), TEXT("."));
				Str_i32toStr(userData->WinGUIVer.minor, 1,
					version + Str_Len(version), 41 - Str_Len(version));

				GetTextExtentPoint(MemDC, version,
					(int)(Str_Len(version)), &textExtent);

				TextOut(
					MemDC,
					client.right - textExtent.cx - 10,
					client.bottom - textExtent.cy - 10,
					version,
					(int)(Str_Len(version))
				);

				SetBkMode(MemDC, prevBKMode);
				SetTextColor(MemDC, prevColor);
				SelectObject(MemDC, prevFont);
				
			}

			BitBlt(hdc, 0, 0, client.right - client.left,
				client.bottom - client.top, MemDC, 0, 0, SRCCOPY);

			SelectObject(MemDC, oldMemBitmap);

			DeleteObject(MemBitmap);
			DeleteDC(MemDC);

			EndPaint(hwnd, &ps);

			return 0;

		}
		
		case WM_GETMINMAXINFO: {

			Main_Window *userData =
				(Main_Window*)(GetWindowLongPtr(hwnd, GWLP_USERDATA));

			if (userData != NULL) {
				((LPMINMAXINFO)(lParam))->ptMinTrackSize.x = userData->initCX;
				((LPMINMAXINFO)(lParam))->ptMinTrackSize.y = userData->initCY;
			}

			return 0;

		}
		
		case WM_DESTROY: {

			Main_Window *userData =
				(Main_Window*)(GetWindowLongPtr(hwnd, GWLP_USERDATA));

			int exitCode = 1;

			if (userData != NULL) {
				exitCode = userData->exitCode;
				if (userData->main != NULL) {
					DeleteObject(userData->main);
				}
				if (userData->paragraph != NULL) {
					DeleteObject(userData->paragraph);
				}
				UnregisterDeviceNotification(userData->hDevNot);
				Rand_Delete(userData->random);
				userData->random = NULL;
				LocalFree(userData->outputText);
				LocalFree(userData);
			}

			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)(NULL));

			KillTimer(hwnd, 0xFF);
			PostMessage(NULL, WM_QUIT, exitCode, 0);

			return 0;

		}
		
	}

	return DefWindowProc(hwnd, uMsg,
		wParam, lParam);

}
