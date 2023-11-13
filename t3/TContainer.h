#pragma once
#include "TWidget.h"

//todo this is a lot like tgroup, should rework tgroup
// todo consume event concept and broadcast
#define ptrdiff(a,b) ((uint8_t*)a - (uint8_t*)(b))

struct TContainer : public TWidget{
	
	int16_t focus_o, first_o;
	//xy
	//todo focus as index? for first->getat(focus)

	TContainer(uint8_t _cpi,TObject &w) {
		first_o =  ptrdiff(this,(TWidget*)&w);
		TWidget *p = find_first_focusable();
		if(p){ focus_o =  ptrdiff(this,p);}	
		cpi=_cpi;
	}

	TWidget* focus(){ return (TWidget*)(focus_o != 0 ? ptrdiff(this,focus_o) : 0);}
	TWidget* first(){ return (TWidget*)(first_o != 0 ? ptrdiff(this,first_o) : 0);}

	bool g_focusable() { return find_first_focusable()!=0;}
	void s_focusable(bool b) {/* depends on child properties */}

	bool g_focused() { return focus()->g_focused();}
	void s_focused(bool b) { 
		if(b) focus_o = ptrdiff(this,find_first_focusable());
		focus()->s_focused(b);		
	}

	bool DrawNeeded(){ return 1;}
	bool g_redraw() { return 1; }

	static int s_redrawh(TObject *w){((TWidget*)w)->s_redraw(1); return 1;}
	void s_redraw(bool b){ if(b){first()->ForEach(s_redrawh);} }

	TWidget* find_first_focusable() {
		TWidget *p = first();
		while (p){
			if(p->g_focusable()) { return p;}
			p = (TWidget*)p->Next();
		}
		return 0;
	}

	static int draw_help(TObject *o){
		TWidget *w= (TWidget*)o;
		if(w->DrawNeeded() ) { w->Draw(); w->s_redraw(0); }
		return 1;
	}

	void Draw() { 
		uint8_t ox, oy;
		ox = lx; oy = ly;
		lx +=x(); ly +=y();
		first()->ForEach(draw_help); 
		lx = ox; ly = oy;
	}

	void HandleEvent(TEvent &e) { focus()->HandleEvent(e); }


	bool HandleHotKey(TEvent &e) {
		//if(hotkey == e.charcode){
		//	// todo rework consume msg, old one should lose focus, this one should get focus
		//}
	}
	bool HandleFocusChange() {
		if (!focus()->HandleFocusChange()) return FocusNext(); 
		return 1;
	}

	bool FocusNext(){
		TWidget *p = (TWidget *)focus()->Next();

		while (p){			
			if (p->g_focusable() && !p->g_focused()) {
				p->s_focused(1);
				focus()->s_focused(0);
				//focus() = p;
				focus_o = ptrdiff(this,p);
				return 1;
			}
			p = (TWidget*)p->Next();		
		}
		//loose focus allow upper widgets to realloc focus
		focus()->s_focused(0);
		return 0;
	}

	static int update(TObject* o){ ((TWidget*)o)->UpdateData(); return 1;}
	void UpdateData() { first()->ForEach(update); }	

};
