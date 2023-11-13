#pragma once
#include "TWidget.h"

struct TVisContainer : public TWidget{
	//TWidget *first, *visw;	//todo
	int16_t visw_o, first_o;
	uint8_t *vis, vis_old;

	TVisContainer(uint8_t *_vis,TObject &w) : vis(_vis),vis_old(*_vis) {
		first_o =  ptrdiff(this,(TWidget*)&w);
		TWidget *visw = (TWidget *)first()->GetAt(*vis);		
		visw_o = ptrdiff(this,visw);

		cpi = 0;
	}

	TWidget* visw() { return (TWidget*)(visw_o != 0 ? ptrdiff(this,visw_o) : 0);}
	TWidget* first(){ return (TWidget*)(first_o != 0 ? ptrdiff(this,first_o) : 0);}

	bool g_focusable() { return visw()->g_focusable(); }
	//void s_focusable(bool b) {/* depends on child properties */}

	bool g_focused() { return visw()->g_focused();}
	void s_focused(bool b) { visw()->s_focused(b);}

	void Draw(){ visw()->Draw();}
	void HandleEvent(TEvent &e) {visw()->HandleEvent(e);}
	bool HandleFocusChange() {return visw()->HandleFocusChange();}

	bool g_redraw() { return TWidget::g_redraw() || visw()->g_redraw(); }

	void UpdateData() {
		if (*vis != vis_old) {
			vis_old = *vis;
			TWidget *visw = (TWidget *)first()->GetAt(*vis);
			visw->s_redraw(1);
			s_redraw(1);
			fredraw = 1;

			visw_o = ptrdiff(this,visw);
			//todo maybe destroy old, create new
		}
		visw()->UpdateData();
	}

	virtual uint8_t GetWidth(){ return 0; } 
	virtual uint8_t GetHeight(){ return 0; } 
};
