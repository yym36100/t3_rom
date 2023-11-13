#pragma once

#include "TObject.h"
#include "TConsole.h"

struct TWidget : public TObject {

	enum {focusable = 1<<0, focused = 1<<1, redraw = 1<<2};

	static TConsole *con;

	uint8_t cpi; //const pool index	
	uint8_t flags;

	static uint8_t lx,ly;
	static bool showCursor;
	static TWidget *foc; //todo remove
	static bool gredraw;
	static bool fredraw;

	static const char **const_pool;

	uint8_t x() {return const_pool[cpi][0];}
	uint8_t y() {return const_pool[cpi][1];}

	uint8_t w() {return const_pool[cpi][2];}
	uint8_t h() {return const_pool[cpi][3];}


	TWidget() : flags(0) {}
	TWidget(uint8_t _cpi) : flags(0),cpi(_cpi) {}

	virtual bool DrawNeeded(){ return fredraw || g_redraw();} //remove

	virtual bool HandleFocusChange() { s_focused(0);return 0;} //remove
	virtual void HandleEvent(TEvent &e) {}


	virtual bool g_redraw() { return ( flags&redraw)!=0; }
	virtual void s_redraw(bool b) { flags &=~redraw; if(b){flags|=redraw;gredraw = 1;} }

	virtual bool g_focusable() { return (flags&focusable)!=0;}
	virtual void s_focusable(bool b) {flags &=~focusable; if(b)flags|=focusable;}

	virtual bool g_focused() { return (flags&focused)!=0;}
	virtual void s_focused(bool b) { if(b!=g_focused()) {s_redraw(1);flags ^=focused;}} // duplicate this

	virtual void UpdateData() {}

	virtual void Draw(){
		con->SetCurosrPos(lx + x(), ly + y());
		if(g_focusable()){
			if (!g_focused()) con->SetColor(0x17);
			else con->SetColor(0x2e);
		}
	}

	virtual uint8_t GetWidth(){ return w(); } 
	virtual uint8_t GetHeight(){ return h(); } 
};
