#include "WinGUI.h"

// ========================================
// Unicode Aware C String Fuctions

_WIN_GUI_API_ size_t String_Length_U(
	const wchar_t *string
) {

	if (string == NULL) {
		return 0;
	}

	size_t i = 0;
	while (string[i] != L'\0') {
		i++;
	}

	return i;

}

_WIN_GUI_API_ size_t String_Length_A(
	const char *string
) {

	if (string == NULL) {
		return 0;
	}

	size_t i = 0;
	while (string[i] != '\0') {
		i++;
	}

	return i;

}

// ========================================



// ========================================
