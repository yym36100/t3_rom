#pragma once
#include "TWidget.h"
#include "TValue.h"

struct TProgressBar : public TWidget {	
	uint8_t *data;
	uint8_t copy;	

	//xywh color art
	//0123 4     5(3) 


	TProgressBar(uint8_t _cpi, uint8_t *pdata) :TWidget(_cpi),data(pdata) {		
		copy = *data;
	}
	

	void UpdateData() { if(copy!= *data){s_redraw(1); copy = *data;} }

	void DrawBar(char* text, uint8_t start, uint8_t mid, uint8_t stop) {
		int ii = 0;
		for(;ii<start;ii++)text[ii] = ' ';
		for(;ii<mid;ii++)  text[ii] = const_pool[cpi][5+0];		
		for(;ii<stop;ii++) text[ii] =const_pool[cpi][5+1];		
		text[ii] = 0;	
	}

	void Draw() {		
		TWidget::Draw();
		con->SetColor(0x17);//todo

		uint8_t barw,to;
		uint16_t i;
		char text[100];
		char text2[5];
		uint8_t width = w();

		switch(const_pool[cpi][5+2]){
				case 'l':
					barw = width -4;
					i = barw* *data/255;
					to = 0;
					DrawBar(text,4,i+4,width);
					break;
				case 'r':
					barw = width -4;
					i = barw* *data/255;
					to = i;
					DrawBar(text,0,i,width);
					break;
				case 'c':
				default:
					barw =width;
					i = barw* *data/255;
					to = (barw)/2;
					DrawBar(text,0,i,barw);
					break;
		}			
		sprintf(text2,"%d%%",100* *data/255);
		if(const_pool[cpi][5+2] == 'c')to -= strlen(text2)/2;
		if(const_pool[cpi][5+2] != 'n'){
			for(int i=0;i<strlen(text2);i++){
				*(text+to +i) = text2[i];
			}
		}
		con->Write(text);
	}
};
