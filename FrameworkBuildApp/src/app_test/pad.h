#ifndef __PAD__
#define __PAD__

#include <types_v7.h>

typedef enum _EPadButton {
	E_PAD_RELEASED = 0,
	E_PAD_BUTTON_A = 1 << 0,
	E_PAD_BUTTON_B = 1 << 1,
	E_PAD_BUTTON_C = 1 << 2,
	E_PAD_BUTTON_D = 1 << 3,
	E_PAD_BUTTON_X = 1 << 4,
	E_PAD_BUTTON_Y = 1 << 5,
	E_PAD_BUTTON_Z = 1 << 6,
	E_PAD_BUTTON_W = 1 << 7
} EPadButton;

typedef struct _Pad {

	uint32 state;
	uint32 prevState;
} Pad;

extern Pad g_PAD;

void padInitialize(const char * cgfFile);
void padDeinitialize();
void padUpdate();

#endif/**/