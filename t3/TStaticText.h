#pragma once
#include "TWidget.h"


struct TStaticText : public TWidget {

	//xywhat
	TStaticText(uint8_t _cpi) {        
		cpi = _cpi;
		if(cpi==14){
			cpi = 14;
		}
	}

	void Draw() {		
		TWidget::Draw();
		con->SetColor(const_pool[cpi][4]);
		con->Write((char*)&const_pool[cpi][5]);		
	}

	uint8_t GetWidth() { return strlen((char*)&const_pool[cpi][5]);}	
};

//
//struct TSCTExt : public TStaticText {
//	TSCTExt(uint8_t x, uint8_t y, char *_text, char _attr = 0x07) : TStaticText(x, y, _text, _attr) {}
//
//	void Draw() {
//		uint8_t len = strlen(text);
//		char *ttext = new char[len+1];
//		char *tattr = new char[len+1]; 
//
//		char *pt = text;
//		bool hl = false;
//		int ii = 0;
//		for (int i = 0; i < len; i++){
//			if (*pt == '~') { hl ^= 1; pt++; continue; }
//			ttext[ii] = *pt++;
//			tattr[ii] = hl ? 0x0a : 0x07;
//			//hl = 0;
//			ii++;
//		}
//		ttext[ii] = 0;
//
//		con->SetCurosrPos(lx+s.x, ly+s.y);
//		con->SetColor(~attr);
//		con->Write(ttext,tattr);
//
//		delete[] ttext;//todo
//		delete[] tattr;
//	}
//	uint8_t GetWidth() { return strlen(text);} //todo exclude ~s	
//};
