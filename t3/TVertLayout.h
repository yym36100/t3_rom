#pragma once
#include "TContainer.h"

struct TVertLayout : public TContainer {	
	TVertLayout(uint8_t _cpi,TObject &w) : TContainer(_cpi,w) {} 

	static int draw_help(TObject *o){ 
		TWidget *w= (TWidget*)o;
		if(w->DrawNeeded()){
			w->Draw();
			w->s_redraw(0);
		}
		ly +=w->GetHeight() + w->y();
		return 1;
	}

	void Draw(){ 
		uint8_t ox, oy;
		ox = lx; oy = ly;
		lx +=x(); ly +=y();
		first()->ForEach(draw_help); 
		lx = ox; ly = oy;
	}
};
