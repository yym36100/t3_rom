#pragma once
#include "TWidget.h"

struct TValue : public TWidget {
	uint32_t *val;
	uint32_t copy;	
	//char hotkey;

	TValue(uint8_t _cpi, uint32_t *pdata, char _hotkey = 0) : val(pdata) {
		s_focusable(1);
//		hotkey = _hotkey;		
		cpi = _cpi;
	}

	uint8_t GetWidth(){ return w(); } 
	


	void UpdateData() { if(copy!= *val){s_redraw(1); copy = *val;} }

	void HandleEvent(TEvent &e) {		
		switch (e.charcode)
		{
		case 0:
			{
				switch(e.keycode)
				{
				case 33:(*val)+=10;/*s_redraw(1);*/break;
				case 34:(*val)-=10;/*s_redraw(1);*/break;
				}
				break;
			}
		case '+': (*val)++;/* Draw(); */break;
		case '-': (*val)--; /*Draw();*/ break;
		case ' ':(*val) = 0;/* Draw();*/ break;
		case 13: 
			break;
		}        
	}

	void Draw() {
		char text[20];
		TWidget::Draw();

		sprintf(text, "%d", *val);		
		con->WriteAlign(text,GetWidth());
	}
};
