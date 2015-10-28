
#include "pad.h"

#include <windows.h>

typedef struct _PadCode {
	uint32 key;
	uint32 button;
} PadCode;

static PadCode s_codeMap[] = {
	{ 'A',		E_PAD_BUTTON_A },
	{ 'D',		E_PAD_BUTTON_B },
	{ 'S',		E_PAD_BUTTON_C },
	{ 'W',		E_PAD_BUTTON_D },
	{ VK_UP,	 E_PAD_BUTTON_X },
	{ VK_DOWN,	E_PAD_BUTTON_Y },
	{ VK_SPACE, E_PAD_BUTTON_Z },
	{ VK_SHIFT, E_PAD_BUTTON_W }
};

static uint32 s_codes = sizeof(s_codeMap) / sizeof(s_codeMap[0]);

Pad g_PAD;

FUNC_DEF_INLINE void padPoll() {

	uint32 i = 0;
	uint32 state = E_PAD_RELEASED;

	for (; i < s_codes; ++i) {

		int32 res = !!(GetAsyncKeyState(s_codeMap[i].key) & 0x8000);
		if (res) state |= s_codeMap[i].button;
	}

	g_PAD.prevState = g_PAD.state;
	g_PAD.state = state;
}

void padInitialize(const char * pCgfFile) {

	g_PAD.state = 0;
	g_PAD.prevState = 0;
}

void padDeinitialize() {

}

void padUpdate() {
	padPoll();
}



