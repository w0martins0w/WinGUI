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
	size_t dest_size,
	const wchar_t *src
) {

	if (dest == NULL) {
		return dest;
	}
	
	if (dest_size != 0) {

		size_t i = 0;
		
		if (src != NULL) {

			while (i < dest_size) {

				if (src[i] == L'\0') {
					i++; break;
				}

				dest[i] = src[i];

				i++;

			}

		} else {

			i++;

		}
		
		dest[i - 1] = L'\0';
		
	}

	return dest;

}

_WIN_GUI_API_ char *StrA_Cpy(
	char *dest,
	size_t dest_size,
	const char *src
) {

	if (dest == NULL) {
		return dest;
	}

	if (dest_size != 0) {

		size_t i = 0;

		if (src != NULL) {

			while (i < dest_size) {

				if (src[i] == '\0') {
					i++; break;
				}

				dest[i] = src[i];

				i++;

			}

		} else {

			i++;

		}

		dest[i - 1] = '\0';

	}

	return dest;

}

// ========================================



// ========================================
