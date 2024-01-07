#ifndef _WIN_GUI_H_
#define _WIN_GUI_H_

#ifdef _WIN_GUI_DLL_EXPORT_
	#define _WIN_GUI_API_ __declspec(dllexport)
#else // _DLL_IMPORT_
	#define _WIN_GUI_API_ __declspec(dllimport)
#endif

#include <stdint.h>
#include <stdlib.h>

#ifdef UNICODE
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

	/// <summary>
	///		WinGUI_Ver Structure
	/// </summary>
	typedef struct _WIN_GUI_VER_ {
		uint32_t version; // Version Number
		uint16_t major;   //   0xFFFF0000
		uint16_t minor;   //   0x0000FFFF
	} WinGUI_Ver;

	/// <summary>
	///		Retrieve "WinGUI" Version Number
	/// </summary>
	/// <param name="self">Pointer to "WinGUI_Ver" Structure</param>
	/// <returns>
	///		TRUE: Success / FALSE: Failure
	/// </returns>
	_WIN_GUI_API_ _Bool WinGUI_GetVer(
		WinGUI_Ver *self
	);

	// ========================================
	// Unicode Aware C String Fuctions

	/// <summary>
	///		Retrieve Unicode String Length
	/// </summary>
	/// <param name="str">Pointer to Unicode String</param>
	/// <returns>
	///		String Length in Characters without L'\0'
	/// </returns>
	_WIN_GUI_API_ size_t StrW_Len(
		const wchar_t *str
	);

	/// <summary>
	///		Retrieve ANSI String Length
	/// </summary>
	/// <param name="str">Pointer to ANSI String</param>
	/// <returns>
	///		String Length in Characters without '\0'
	/// </returns>
	_WIN_GUI_API_ size_t StrA_Len(
		const char *str
	);

	#ifdef UNICODE
		#define Str_Len StrW_Len
	#else // ANSI
		#define Str_Len StrA_Len
	#endif

	_WIN_GUI_API_ wchar_t *StrW_Cpy(
		wchar_t *dest,
		size_t size,
		const wchar_t *src
	);

	_WIN_GUI_API_ char *StrA_Cpy(
		char *dest,
		size_t size,
		const char *src
	);

	#ifdef UNICODE
		#define Str_Cpy StrW_Cpy
	#else // ANSI
		#define Str_Cpy StrA_Cpy
	#endif

	_WIN_GUI_API_ wchar_t *StrW_Cat(
		wchar_t *dest,
		size_t size,
		const wchar_t *src
	);

	_WIN_GUI_API_ char *StrA_Cat(
		char *dest,
		size_t size,
		const char *src
	);

	#ifdef UNICODE
		#define Str_Cat StrW_Cat
	#else // ANSI
		#define Str_Cat StrA_Cat
	#endif

	_WIN_GUI_API_ StrW_i32toStr(
		int32_t i32,
		_Bool inUnsigned,
		wchar_t *dest,
		size_t size
	);

	_WIN_GUI_API_ StrA_i32toStr(
		int32_t i32,
		_Bool inUnsigned,
		char *dest,
		size_t size
	);

	#ifdef UNICODE
		#define Str_i32toStr StrW_i32toStr
	#else // ANSI
		#define Str_i32toStr StrA_i32toStr
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
