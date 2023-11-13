#pragma once
#include "TWidget.h"

#define pattern2 "\xb0\xb1\xb2\xb1"
#define pattern "0100101010111011"

struct TBackground : public TWidget {
	//xywhap
	//012345
	TBackground(uint8_t _cpi) : TWidget(_cpi) {}

	void Draw(){		
		uint16_t xs = lx + x(), ys = ly + y();
		char linebuff[100+1];

		con->SetColor(rand());

		const char *pp = &const_pool[cpi][5];
		int pl = strlen(pp);
		int pi = 0;

		for(int i=0;i<h();i++){
			for(int j=0;j<w();j++){
				linebuff[j] = pp[pi++];
				if(pi==pl) pi = 0;
			}
			linebuff[w()] = 0;
			con->SetCurosrPos(xs, ys+i);
			con->Write(linebuff);
		}
		linebuff;
	}

	//uint8_t GetWidth(){ return e.x - s.x + 1; } 
	//uint8_t GetHeight(){ return e.y - s.y + 1; } 
};
