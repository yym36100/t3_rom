#pragma once
#include "TWidget.h"

struct TGroup : public TWidget{
	TWidget *first;

	TGroup(TObject &w) : first((TWidget*)&w) {}


	void Draw(){}


	static int draw_help(TObject* o){ ((TWidget*)o)->Draw(); return 1;}
	static int update_help(TObject* o){ ((TWidget*)o)->UpdateData(); return 1;}

	void DrawAll() {		
		lx = ly = 0;
		fredraw = 1;
		first->ForEach(update_help);
		first->ForEach(draw_help);
		con->RestoreCursor();
		fredraw = 0;
	}

	void Init() {
		// set first one to focused		
		TWidget *p = first;
		while (p){
			if (p->g_focusable()){
				p->s_focused(1);
				//focus = p;
				foc = p;
				break;
			}
			p = (TWidget*)p->Next();
		}
		
		DrawAll();
	}

	void AllHandleEvent(TEvent &e){ if(foc) foc->HandleEvent(e); }

	void FocusNext(){		
		TWidget *p = (TWidget *)foc->Next();
		uint8_t cnt=1;
		if(!p) {p = first; cnt = 0;}
		
		while (p){			
			if (p->g_focusable() && !p->g_focused()) {
				p->s_focused(1);
				//focus->s_focused(0);
				foc = p;
				break;
			}
			p = (TWidget*)p->Next();
			if (!p){
				p = first;
				if(cnt--==0)break;
			}
		}
	}

	/*bool AllHandleHotkey(TEvent& e){
		TWidget *p = first;
		while (p){
			if(p->g_focusable()) {
				if( p->hotkey == e.charcode){
				foc->s_focused(0);
				p->s_focused(1);
				foc = p;
				return true;		
				}
			}
			p = (TWidget*)p->Next();
		}
		return false;
	}

	bool AllHandleHotkey2(TEvent& e){
		TWidget *p = first;
		while (p){			
			if(p->g_focusable()) {
				if( p->hotkey == e.charcode){
				foc->s_focused(0);
				p->s_focused(1);
				foc = p;
				return true;		
				}
			}
			p = (TWidget*)p->Next();
		}
		return false;
	}*/

	static int drawupdate_help(TObject* o){ 
		TWidget *w= (TWidget*)o;
		if(w->DrawNeeded() ) { w->Draw(); w->s_redraw(0); }
		return 1;
	}

	void DrawUpdate(void){	
		first->ForEach(update_help);
		if(gredraw || fredraw){
			lx = ly = 0;
			first->ForEach(drawupdate_help);
			gredraw = 0;
		}
		fredraw=0;
	}

	bool Run() {
		//Init();
		//DrawAll();

		TEvent e;
		//while (1)
		{
			
			lx = ly = 0;
			showCursor = 0;

			e.charcode = 255;
			e.keycode= 255;
			con->GetKeyEvent(e);
			con->SetColor(7);
			
			if(e.charcode ==255 && e.keycode==255) return 1;			
			if(e.charcode == 'k'){
			con->Clear();
			//Sleep(50);
			}
			
			/*if(AllHandleHotkey2(e)){
				DrawAll();
				return 1;
			}*/

			switch (e.charcode){
			case 'q': return 0;
			case '\t':	
				if(foc)
				if (!foc->HandleFocusChange()) FocusNext(); 
				//DrawAll();
				
				break;
			default: AllHandleEvent(e);
				//Sleep(20);
			}
			con->ShowCursor(showCursor);
			con->RestoreCursor();

		}
		return 1;
	}
};
