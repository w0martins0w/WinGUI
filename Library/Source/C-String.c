#include "WinGUI.h"

// ========================================
// Unicode Aware C String Fuctions

_WIN_GUI_API_ size_t StrW_Len(
	const wchar_t *str
) {

	if (str == NULL) {
		return 0;
	}

	size_t i = 0;
	while (str[i] != L'\0') {
		i++;
	}

	return i;

}

_WIN_GUI_API_ size_t StrA_Len(
	const char *str
) {

	if (str == NULL) {
		return 0;
	}

	size_t i = 0;
	while (str[i] != '\0') {
		i++;
	}

	return i;

}

_WIN_GUI_API_ wchar_t *StrW_Cpy(
	wchar_t *dest,
	size_t size,
	const wchar_t *src
) {

	if (dest == NULL) {
		return dest;
	}
	
	const wchar_t *cpy_str =
		(src == NULL) ? L"" : src;

	size_t i = 0;
	while (i < size) {

		dest[i] = cpy_str[i];

		if (cpy_str[i] == L'\0') {
			break;
		}

		i++;

	}

	if (size != 0 && i == size) {
		dest[i - 1] = L'\0';
	}

	return dest;

}

_WIN_GUI_API_ char *StrA_Cpy(
	char *dest,
	size_t size,
	const char *src
) {

	if (dest == NULL) {
		return dest;
	}

	const char *cpy_str =
		(src == NULL) ? "" : src;

	size_t i = 0;
	while (i < size) {

		dest[i] = cpy_str[i];

		if (cpy_str[i] == '\0') {
			break;
		}

		i++;

	}

	if (size != 0 && i == size) {
		dest[i - 1] = '\0';
	}

	return dest;

}

_WIN_GUI_API_ wchar_t *StrW_Cat(
	wchar_t *dest,
	size_t size,
	const wchar_t *src
) {

	if (dest == NULL) {
		return dest;
	}

	size_t i = 0;
	while (i < size) {

		if (dest[i] == L'\0') {
			StrW_Cpy(dest + i, size - i, src);
			return dest;
		}

		i++;

	}

	return dest;

}

_WIN_GUI_API_ char *StrA_Cat(
	char *dest,
	size_t size,
	const char *src
) {

	if (dest == NULL) {
		return dest;
	}

	size_t i = 0;
	while (i < size) {

		if (dest[i] == '\0') {
			StrA_Cpy(dest + i, size - i, src);
			return dest;
		}

		i++;

	}

	return dest;

}

_WIN_GUI_API_ StrW_i32toStr(
	int32_t i32,
	_Bool inUnsigned,
	wchar_t *dest,
	size_t size
) {

	if (dest == NULL) {
		return /* void */;
	}

	// Assume "i32" is Unsigned
	uint32_t number = i32;

	if (!inUnsigned && i32 < 0) {
		number = i32 * -1;
	}

	int8_t digits_i;
	wchar_t digits[10];
	for (digits_i = 9; digits_i >= 0; digits_i--) {
		digits[digits_i] = (number % 10) + '0';
		number /= 10;
		if (digits[digits_i] == '0') {
			break;
		}
	}

	if (digits_i != 9) {
		digits_i++;
	}

	size_t i = 0;
	while (i < size) {

		if (i != 0) {
			if (digits_i == 10) {
				i++;
				break;
			}
		}

		if (i == 0) {
			if (!inUnsigned && i32 < 0) {
				dest[i] = '-';
				i++;
				continue;
			}
		}
		
		dest[i] = digits[digits_i];

		digits_i++;
		i++;

	}

	if (size != 0) {
		dest[i - 1] = L'\0';
	}

}
