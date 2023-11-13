#pragma once

#define funky "-O|ooO  "
#define funky2 "\xb2\xb2\xb2\xb2\xb2\xb2  "
#define funky21 "\xb1\xb1\xb1\xb1\xb1\xb1  "
#define funky22 "\xb0\xb0\xb0\xb0\xb0\xb0  "
#define funky3 "-O|ooO  "
#define basic "xxxxxx  "
#define simple "\xc4\xbf\xb3\xd9\xc0\xda  "
#define doublel "\xcd\xbb\xba\xbc\xc8\xc9  "

struct TFrame : public TWidget {

	//xywh attr art title
	//0123 4    5   13  
	TFrame(uint8_t _cpi) : TWidget(_cpi) {}

	void Draw(){

		uint8_t xx = x();
		uint8_t yy = y();

		char *linebuff = new char[w()+1];
		con->SetCurosrPos(lx + x(), ly + y());
		con->SetColor(const_pool[cpi][4]);

		for (int i = 1; i < w() - 1; i++) { linebuff[i] = const_pool[cpi][5]; }
		linebuff[0] = const_pool[cpi][5+5]; linebuff[w() - 1] = const_pool[cpi][5+1]; linebuff[w()] = 0;
		char *linebuff_a = new char[w()+1];
		memset(linebuff_a,const_pool[cpi][4],w()+1);
		const char  *pt = &const_pool[cpi][13];
		if (pt) {

			int tl = strlen(pt);
			if (tl > w() - 4) tl = w() - 4;			
			int offs = ((w() - 4) - tl) / 2;
			for (int i = 0; i < tl; i++) {
				linebuff[2 + offs + i] = *pt++;
				linebuff_a[2 + offs + i] = 0x70;
			}
			linebuff[2 + offs - 1] = const_pool[cpi][5+6];
			linebuff[2 + offs + tl] = const_pool[cpi][5+7];
		}
		con->Write(linebuff,linebuff_a);
		if(linebuff_a) delete[] linebuff_a;

		con->SetCurosrPos(lx + x(), ly + y()+ h() -1);
		for (int i = 1; i < w() - 1; i++) { linebuff[i] = const_pool[cpi][5]; }
		linebuff[0] = const_pool[cpi][5+4]; linebuff[w() - 1] = const_pool[cpi][5+3]; linebuff[w()] = 0;
		con->Write(linebuff);

		linebuff[1] = 0;
		for (int i = 1; i < h() - 1; i++) {
			linebuff[0] = const_pool[cpi][5+2];
			con->SetCurosrPos(lx+x(), ly+y() + i);			
			con->Write(linebuff);
			con->SetCurosrPos(lx+x() + w() - 1, ly+y() + i);			
			con->Write(linebuff);
		}
		delete[] linebuff;
	}	
};
