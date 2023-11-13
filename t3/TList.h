#pragma once
#include "TSymbol.h"

struct TList : public TSymbol {
	//xywh
	TList(uint8_t _cpi, uint8_t _spi, uint8_t *_idx,char _hotkey=0) : TSymbol(_cpi, _spi, _idx,_hotkey) {        
		cpi = _cpi;
	}

	void Draw() {
		uint8_t i;
		for( i=0;i<NoItems();i++){
			con->SetCurosrPos(lx+x(),ly+y()+i);						
			con->SetColor( i!=*idx ? 0x17 : g_focused() ?  0x2e : 0x4e);
			con->WriteAlign(GetAt(i),w(),TConsole::right);
		}		
	}
};
