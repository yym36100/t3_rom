#pragma once
#include "TWidget.h"
#include "TFrame.h"
#include "TBackground.h"

struct TWindow: public TFrame {	
	TWindow(uint8_t _cpi) : TFrame(_cpi) {}

	void Draw() {
		TFrame::Draw();
		// clear interior		
		uint8_t xs = lx + x()+1, ys = ly + y()+1;
		char linebuff[80];
		memset(linebuff,' ',w()-2);
		linebuff[w()-2] = 0;

		con->SetColor(rand());		

		for(int i=0;i<h()-2;i++){
			con->SetCurosrPos(xs, ys+i);
			con->Write(linebuff);
		}
	}
};
