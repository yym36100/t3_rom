#pragma once
#include "TWidget.h"
#include "TValue.h"

#define sb_simple "<-*->"
#define sb_simple1 "\x11\xc4\xf0\xc4\x10"
#define sb_simple2 "\x11\xcd\xf0\xcd\x10"

#define sb_classic1 "\x11\xb0\xfe\xb0\x10"
#define sb_classic1f "\x11\xdb\xdb\xb0\x10"
#define sb_classic2 "\x11\xb1\xfe\xb1\x10"
#define sb_classic3 "\x11\xb2\xfe\xb2\x10"

struct TScroll : public TValue {

	//xywhca
	TScroll(uint8_t _cpi, uint32_t *pdata, char _hotkey = 0) : TValue(_cpi,pdata,_hotkey) {}

	void Draw() {
		TWidget::Draw();
		if(g_focused())con->SetColor(0x02);
		else con->SetColor(0x12);

		uint32_t val2 = *val%100;
		uint16_t i = w()* val2/100;
		

		char text[51];

		text[0] =  const_pool[cpi][5+0]	;
		for(int ii=1;ii<i;ii++) text[ii] = const_pool[cpi][5+1];
		text[i] = const_pool[cpi][5+2];
		for(int ii=i+1;ii<w();ii++) text[ii] = const_pool[cpi][5+3];
		text[w()-1] =  const_pool[cpi][5+4];
		text[w()] = 0;

		con->Write(text);
	}
};
