#include "stdafx.h"
#include <conio.h>
#include <stdlib.h>

#include "TConsole.h"
#include "TStaticText.h"
#include "TValue.h"
#include "TGroup.h"
#include "TSymbol.h"
#include "TFrame.h"
#include "TMultiLine.h"
#include "TScroll.h"
#include "TList.h"
#include "TContainer.h"
#include "TVisContainer.h"
#include "TLinearLayout.h"
#include "TBackground.h"
#include "TWindow.h"
#include "TLineInput.h"
#include "TProgressBar.h"
#include "TVertLayout.h"


#define helptext "Change selection with the +/- keys\n"\
	"Move focus with the TAB key\n\n"\
	"Configure Channel A parameters here"\
	"Line 1\n"\
	"Line 2\n"\
	"Line 3\n"\
	"Last line\n"

TConsole *TWidget::con;
uint8_t TWidget::lx,TWidget::ly;
TWidget* TWidget::foc = 0;
bool TWidget::showCursor = 0;
bool TWidget::gredraw = 0;
bool TWidget::fredraw = 0;
//bool TContainer::f = 0;

char TLineInput::tmptext[50];

static uint32_t totalsize = 0;
static uint32_t dtotalsize = 0;

char bigarray[16*1024];

void* operator new  ( size_t count ){
	
	count /=4;
	count +=2;
	count*=4;
	static char *myheap = 0;
	if(!myheap) myheap=(char*)bigarray;
	totalsize+=count;
	return (myheap + totalsize);
}

void operator delete(void * p) {
	dtotalsize++;
	//free(p);
}

char sizetext[100] = "\0\x17  \7";

const char *_const_pool[] = {
	/* 0 TBackground */ "\0\0\x3c\x19\7""0100101010111011",	
	/* 1 Stattext    */ sizetext,
	/* 2 TFrame       */"\0\0\xe\xa\x1f"simple"Menu",
	/* 3  Tcontainer*/"\6\2",
	/* 4 tlist */"\1\2\xc\x8",
	/* 5 tcont pos00*/"\0\0",
	/* 6 Stattext    */"\0\0  \x41semmi",
	/* 7  Tcontainer*/"\xe\0",
	/* 8 Stattext    */"\0\0  \x42valami",
	/* 9 Stattext    */"\0\0  \x42harmadik",
	/* 10 Stattext    */"\0\0  \x42""4",
	/* 11 TFrame       */"\0\0\x1e\xa\x1f"simple"Channel A Config",
	/* 12 vert layout  */"\2\2",
	/* 13 Stattext    */"\0\0  \7""Amplitude:",
	/* 14 Stattext    */"\0\0  \7""mV",
	/* 15 value       */"\0\0\xc",
	/* 16 Stattext    */"\0\0  \7""Frequency:",
	/* 17 Stattext    */"\0\0  \7""Hz",
	/* 18 Stattext    */"\0\0  \7""Signal   :",
	/* 19 symbol */"\0\0\xc",
	/* 20 TFrame       */"\0\0\x2d\xa\x1f"simple"Help",
	/* 21 multiline       */"\1\1\x2a\x4\x1f"helptext,
	/* 22  Tcontainer*/"\0\xa",
	/* 23  progress */"\1\6\x2b\1\x1f""\xdb\xb0""c",	
	/* 24  scroll */"\1\5\x2b\1\x1f""\x1b\xc4\x7f\xc4\x1a",
	/* 25  progress */"\1\x8\x2b\1\x1f""= r",

	/* 26 Stattext    */"\0\0  \7""Phase    :",
	/* 27 Stattext    */"\0\0  \7""\xf8",
	/* 28 TFrame       */"\0\0\x1e\xa\x1f"simple"Miscellanious",

	/* 29 Stattext    */"\0\0  \7""Resolution:",
	/* 30 Stattext    */"\0\0  \7""Out. buff.:",
	/* 31 Stattext    */"\0\0  \7""bit",
	/* 32 tcont pos00*/"\2\2",
	/* 33 tcont pos00*/"\2\4",

	/* 34 TFrame       */"\0\0\x1e\x14\x1f"doublel"About",

	/* 35 Stattext    */"\2\2  \7""myTV demonstrator",
	/* 36 Stattext    */"\2\4  \7""sado TECH 2017",

	/* 37 Stattext     */"\2\6  \7""           _____ _____ ",
	/* 38 Stattext     */"\2\7  \7"" _____ _ _|_   _|  |  |",
	/* 39 Stattext    */"\2\x8  \7""|     | | | | | |  |  |",
	/* 40 Stattext    */"\2\x9  \7""|_|_|_|_  | |_|  \\___/ ",
	/* 41 Stattext    */"\2\xa  \7""      |___|            ",

	/* 42 Stattext    */"\2\xe  \7""bit",
};

const char *_symb_pool1[] = { "Channel A","Channel B","Options","About",0};
const char *_symb_pool2[] = { "sin","cos","square","triangle",0};
const char *_symb_pool3[] = { "On","Off",0};
const char **_symb_pool[] = { _symb_pool1,_symb_pool2,_symb_pool3 };

const char  **TWidget::const_pool = _const_pool;
const char ***TSymbol::symb_pool = _symb_pool; 

bool volatile dbg = 1;
uint8_t dgbcnt = 0;

int _tmain(int argc, _TCHAR* argv[])
{
	TConsole c(60, 25);
	TWidget::con = &c;

	TGroup *w;
	uint32_t data1 = 36;
	uint32_t data2 = 100;
	uint32_t data3 = 100;
	uint32_t data4 = 100;
	uint8_t idx = 0;
	uint8_t idx2 = 0;
	uint8_t viscnt = 3;
	uint8_t menu = 0;
	uint8_t progbar=0;

	char text[100] = "hello";
	char text2[100] = "second";
	char text3[100] = "harom";
	char edittext[100] = {0};

	char lineinp[50] = "default";

	//while(dbg){
	//	dgbcnt++;
	//}

	w = new TGroup(
		*new TBackground(0)
		+*new TStaticText(1)		
		+*new TContainer(3,
		*new TWindow(2)
		+*new TList(4,0,&menu,'m')
		+*new TVisContainer(&menu,
		*new TContainer(5,
		*new TContainer(7,
		*new TWindow(11)
		+*new TVertLayout(12,
		*new TLinearLayout(5,*new TStaticText(13)+*new TValue(15,&data1)+*new TStaticText(14)) 
		+*new TLinearLayout(5,*new TStaticText(16)+*new TValue(15,&data2)+*new TStaticText(17)) 
		+*new TLinearLayout(5,*new TStaticText(18)+*new TSymbol(19,1,&idx2))							
		)		
		)
		+*new TContainer(22,
		*new TWindow(20)
		+*new TMultiLine(21)
		+*new TProgressBar(23,&progbar)
		+*new TScroll(24,&data1)
		+*new TProgressBar(25,&progbar)
		)
		)
		+*new TContainer(5,					
		*new TContainer(7,
		*new TWindow(11)
		+*new TVertLayout(12,
		*new TLinearLayout(5,*new TStaticText(13)+*new TValue(15,&data1)+*new TStaticText(14)) 
		+*new TLinearLayout(5,*new TStaticText(16)+*new TValue(15,&data2)+*new TStaticText(17)) 
		+*new TLinearLayout(5,*new TStaticText(18)+*new TSymbol(19,1,&idx2))
		+*new TLinearLayout(5,*new TStaticText(26)+*new TValue(15,&data1)+*new TStaticText(27))
		)		
		)
		)
		+*new TContainer(5,
		*new TContainer(7,
		*new TWindow(28)
		+*new TLinearLayout(32,*new TStaticText(29)+*new TValue(15,&data1)+*new TStaticText(31))
		+*new TLinearLayout(33,*new TStaticText(30)+*new TSymbol(15,2,&idx2))
		)
		)
		+*new TContainer(5,
		*new TContainer(7,
		*new TWindow(34)
		+*new TStaticText(35)
		+*new TStaticText(36)

		+*new TStaticText(37)
		+*new TStaticText(38)
		+*new TStaticText(39)
		+*new TStaticText(40)
		+*new TStaticText(41)
		+*new TLineInput(42,lineinp)	
		)
		)
		)				
		)
		);

	c.Clear();	
	c.ShowCursor(false);

	

	sprintf(sizetext+5,"O %d W %d LnInp %d tot %d (2028)", sizeof(TObject),sizeof(TWidget),sizeof(TLineInput), totalsize/*, typeid(w).name()*/);

	w->Init();
	while(w->Run()){
		//menu=255;
		//menu++;
		progbar++;
		data1++;

		//w->gredraw=1;
		w->DrawUpdate();
		/*	if(menu==0)Sleep(2000);
		if(menu==30)Sleep(2000);
		if(menu==254)Sleep(2000);
		if(menu==255)Sleep(2000);*/
		Sleep(20);
	}

	return 0;
}





