#pragma once
#include "TWidget.h"

struct TMenu : public TWidget {
	char *text;
	TMenu *sub;

	TMenu(uint8_t x, uint8_t y, char *_text, char _attr = 0x7) : text(_text), attr(_attr) { 
		/*s_focusable(1);
		hotkey = _hotkey;
		s.x = x; s.y = y;*/
	}
	void Draw() {}
}; 
