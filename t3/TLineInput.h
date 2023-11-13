#pragma once
#include "TWidget.h"

//todo fix keys
struct TLineInput : public TWidget {

public:
	char *text;
	static char tmptext[50];
	uint8_t i;
	uint8_t state;
	enum {idle=0,focused, editing};
	enum {max_len=7};
	bool insert;	
	uint8_t hotkey;
		

	TLineInput(uint8_t _cpi, char *tt, char _hotkey = 0) : text(tt),state(idle) {
		s_focusable(1);
		hotkey = _hotkey;
		cpi = _cpi;
		
		memset(tmptext,0,50);
		i = 0;
		insert = 0;
	}

	void s_focused(bool f){
		TWidget::s_focused(f);
		if(f) state = focused; else state = idle;
	}

	void HandleEvent(TEvent &e) {		
		if(state == editing){

			if(e.keycode == 45) {
				//toggle insert mode
				insert ^=1;
				con->SetCursorMode(insert);
				s_redraw(1);
			}
			if(e.keycode == 46) {
				//delete
			}
			//36 35 home end
			if(e.keycode == 36){
				i=0;
				s_redraw(1);
			}
			if(e.keycode == 35){
				i=strlen(tmptext);
				s_redraw(1);
			}
			// 37 <<< 39>>>
			if(e.keycode == 37){
				if(i>0)i--;
				s_redraw(1);
			}
			if(e.keycode == 39){
				if(i<max_len)i++;
				s_redraw(1);
			}

			if(e.charcode == 13 || e.charcode == 9/* enter or tab*/){
				strcpy(text, tmptext);
				tmptext[i=0]=0;
				state = focused;
				s_redraw(1);
			} else if( e.charcode == 27 /* escape */){
				tmptext[i=0]=0;
				state = focused;
				s_redraw(1);
			} else if( e.charcode >=' ') { // todo add filtering
				if(strlen(tmptext)<=max_len){
					if(insert==0)
						memmove(&tmptext[i]+1,&tmptext[i],strlen(&tmptext[i]));
					tmptext[i++]=e.charcode;
					//tmptext[i]=0;
					s_redraw(1);
				}
			} else if (e.charcode ==  8 /*delete */)
			{
				if(i>0){
					tmptext[--i] = 0;
					s_redraw(1);
				}
			}

		}else{
			//handle enter
			if(e.charcode == 13 || e.charcode == ' '){
				state = editing;
				i = 0;
				//tmptext[0]=0;
				memset(tmptext,0,50);
				s_redraw(1);
			}
		}
	}

	void Draw() {	
		TWidget::Draw();		

		con->ShowCursor(state == editing);
		if(state == editing){ showCursor = 1;
		con->SetVisCurosrPos(lx + x()+i, ly + y());
		}

		con->SetCurosrPos(lx+x(),ly+y());
		con->WriteAlign(state!=editing? text : tmptext,8,TConsole::left);
	}

};
