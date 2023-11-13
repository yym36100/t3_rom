#pragma once
#include "TWidget.h"

struct TMultiLine : public TWidget {
	//char *text;
	//char attr;
	uint8_t yoffset;
	//TPoint s,e;
	//xywha text

	TMultiLine(uint8_t _cpi) :TWidget(_cpi),yoffset(0) {
		s_focusable(1);			
	}

	uint8_t GetWidth(){  return w(); } 
	uint8_t GetHeight(){ return h(); } 
	 
	char* GetLine(uint8_t line){
		char *p = (char*)&const_pool[cpi][5];
		while(line--) while(*p) if(*p++=='\n') break;
		return p;
	}

	uint8_t GetNoLines(){ 
		char *p = (char*)&const_pool[cpi][5];
		uint8_t res = 0;
		while(*p) if(*p++=='\n') res++;
		return res;
	}

	void HandleEvent(TEvent &e) {
		switch (e.charcode){
		case '+': if(yoffset<(GetNoLines()-GetHeight()) ){yoffset++; s_redraw(1);} break;
		case '-': if(yoffset>0){yoffset--;s_redraw(1); } break;
		}        
	}

	void Draw() {
		char *linebuff = new char[GetWidth()+1];
		char *p = GetLine(yoffset);

		if(g_focused()) con->SetColor(0x1e);
		else con->SetColor(0x07);

		int xx, yy;
		for(yy=0;yy<h() && *p;yy++){
			for(xx=0;xx<w() && *p;xx++){
				if(*p == '\n') { p++;break; }
				linebuff[xx] = *p++;
			}
			for(;xx<w();xx++) linebuff[xx] = ' ';
			linebuff[xx] = 0;
			con->SetCurosrPos(lx+x(), ly+y()+yy);		
			con->Write(linebuff);		
		}

		for(;yy<h();yy++){
			for(xx=0;xx<w()-1;xx++) linebuff[xx] = ' ';
			linebuff[xx] = 0;
			con->SetCurosrPos(lx+x(), ly+y());		
			con->Write(linebuff);		
		}
		delete[] linebuff;
	}
};
