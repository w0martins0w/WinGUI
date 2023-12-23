#pragma comment(linker,"\
	\"/manifestdependency:type='win32' \
	name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
	processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"\
")

#include <stdint.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

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
	COLORREF textColor;
	hRand random;
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
		WS_OVERLAPPEDWINDOW,
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

			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)(userData));

			if (SetTimer(hwnd, 0xFF, 1000, NULL) == 0) {
				return -1;
			}

			PostMessage(hwnd, WM_TIMER, 0xFF, NULL);

			return 0;
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
					(int)(StrLen(userData->outputText)), &textExtent);

				TextOut(
					MemDC,
					(client.right - client.left) / 2 - textExtent.cx / 2,
					(client.bottom - client.top) / 2 - textExtent.cy / 2,
					userData->outputText,
					(int)(StrLen(userData->outputText))
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
