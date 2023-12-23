#ifndef _WIN_GUI_H_
#define _WIN_GUI_H_

#ifdef _WIN_GUI_DLL_EXPORT_
	#define _WIN_GUI_API_ __declspec(dllexport)
#else // _DLL_IMPORT_
	#define _WIN_GUI_API_ __declspec(dllimport)
#endif

#include <stdint.h>
#include <stdlib.h>

#if defined(UNICODE)
	#ifndef TEXT
		#define TEXT(quote) L##quote
	#endif
#else // ANSI
	#ifndef TEXT
		#define TEXT(quote) quote
	#endif
#endif

typedef unsigned char byte_t;

#ifdef __cpluplus
extern "C" {
#endif

	// ========================================
	// Version

	typedef struct _WIN_GUI_VER_ {
		uint32_t version; // Version Number
		uint16_t major;   //   0xFFFF0000
		uint16_t minor;   //   0x0000FFFF
	} WinGUI_Ver;

	/// <summary>
	///		Retrieve "WinGUI" Version Number
	/// </summary>
	/// <param name="self">Pointer to "WinGUI_Ver" Struct</param>
	/// <returns>
	///		TRUE: Success / FALSE: Failure
	/// </returns>
	_WIN_GUI_API_ _Bool WinGUI_GetVer(WinGUI_Ver *self);

	// ========================================
	// Unicode Aware C String Fuctions

	/// <param name="string">Pointer to Unicode String</param>
	/// <returns>String Length in Characters without L'\0'</returns>
	_WIN_GUI_API_ size_t StrLenW(
		const wchar_t *string
	);

	/// <param name="string">Pointer to ANSI String</param>
	/// <returns>String Length in Characters without '\0'</returns>
	_WIN_GUI_API_ size_t StrLenA(
		const char *string
	);

	#if defined(UNICODE)
		#define StrLen StrLenW
	#else // ANSI
		#define StrLen StrLenA
	#endif

	// ========================================
	// Subtractive Random Number Generator

	/// <summary>
	///		Handle to SRNG
	/// </summary>
	typedef void *hRand;

	/// <param name="seed"></param>
	/// <returns></returns>
	_WIN_GUI_API_ hRand Rand_Create(
		uint32_t seed
	);

	/// <param name="handle"></param>
	/// <returns></returns>
	_WIN_GUI_API_ uint32_t Rand_Next(
		hRand handle
	);

	/// <param name="handle"></param>
	/// <returns></returns>
	_WIN_GUI_API_ double Rand_NextFloat(
		hRand handle
	);

	/// <param name="handle"></param>
	/// <returns></returns>
	_WIN_GUI_API_ void Rand_Delete(
		hRand handle
	);

	// ========================================

#ifdef __cpluplus
}
#endif

#endif // !_WIN_GUI_H_
