#include "WinGUI.h"

/// <summary>
///		Internal SRNG Structure
/// </summary>
typedef struct _Rand {
	uint32_t current;
	uint32_t rand[55];
} Rand;

_WIN_GUI_API_ hRand Rand_Create(
	uint32_t seed
) {
	
	Rand *self = (Rand*)(calloc(1, sizeof(Rand)));

	if (self == NULL) {
		return NULL;
	}

	uint32_t _2 = 0, _1 = seed % UINT32_MAX;

	self->rand[(34 * 0) % 55] = _2;
	self->rand[(34 * 1) % 55] = _1;

	// Initialize SRNG with Initial Values
	for (size_t i = 2; i < 55; i++) {
		uint32_t _0 = (_2 - _1) % UINT32_MAX;
		self->rand[(34 * i) % 55] = _0;
		_2 = _1, _1 = _0;
	}

	self->current = 54;

	// Starting BIAS in Sequence
	for (size_t i = 0; i < 165; i++) {
		Rand_Next((hRand)(self));
	}

	return (hRand)(self);

}

_WIN_GUI_API_ uint32_t Rand_Next(
	hRand handle
) {

	if (handle == NULL) {
		return 0;
	}

	Rand *self = (Rand*)(handle);

	self->current =
		(self->current + 1) % 55;

	self->rand[self->current] =
		(self->rand[((self->current - 54) - (UINT32_MAX - 54)) % 55] -
			self->rand[((self->current - 25) - (UINT32_MAX - 54)) % 55]) %
		UINT32_MAX;

	return self->rand[self->current];

}

_WIN_GUI_API_ double Rand_NextFloat(
	hRand handle
) {

	if (handle == NULL) {
		return 0.0;
	}

	return (
		(double)(Rand_Next(handle)) / (double)(UINT32_MAX)
	);

}

_WIN_GUI_API_ void Rand_Delete(
	hRand handle
) {

	if (handle != NULL) {
		free((Rand*)(handle));
	}

}
