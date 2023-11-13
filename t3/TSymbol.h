#pragma once
#include "TWidget.h"

struct TSymbol : public TWidget {
	uint8_t *idx;	
	uint8_t copy;
	//char hotkey;

	static const char ***symb_pool; 
	uint8_t spi;

	TSymbol(uint8_t _cpi, uint8_t _spi, uint8_t *_idx,char _hotkey=0) : TWidget(_cpi), spi(_spi),idx(_idx){
		s_focusable(1);				
		copy = *idx;
	}

	uint8_t NoItems(){
		uint8_t res = 0;
		while(GetAt(res)) res++;
		return res;
	}

	char *GetAt(uint8_t i){				
		return (char*)symb_pool[spi][i];
	}

	void UpdateData() { if(copy!= *idx){s_redraw(1); copy = *idx;} }

	void HandleEvent(TEvent &e) {	
		switch (e.charcode){
		case '+': (*idx)++; if (*idx >= NoItems())*idx = 0; /*Draw();*/ return;
		case '-': (*idx)--; if (*idx >= NoItems())*idx = NoItems()-1; /*Draw();*/return;
		case ' ':
		case 13: if(NoItems() == 2) *idx^=1; return;
		}
		switch(e.keycode){
			case 36:/* home */ *idx = 0; return;
			case 35:/* end */ *idx = NoItems()-1; return;
		}
	}

	void Draw() {
		TWidget::Draw();        
		uint8_t i = *idx, noi = NoItems();
		if(i>=noi) i = noi-1;

		char *p = GetAt(i);
		con->WriteAlign(p,GetWidth());
	}

	uint8_t GetWidth(){ return w(); } 
	uint8_t GetHeight(){ return h(); } 
};
