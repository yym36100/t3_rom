#pragma once

#define WIN32_LEAN_AND_MEAN
#include "stdint.h"

#include <windows.h>

#include "TObject.h"

class TConsole {
public:
	enum align{left=0, right, center};
    uint8_t w, h, x, y, fg, bg;  
	uint8_t vx,vy;
    HANDLE hcsb, hStdOut, hStdIn;
    uint16_t saveScreenWidth, saveScreenHeight;
    DWORD saveScreenConsoleMode;
	CONSOLE_CURSOR_INFO savecurosrinfo;
	bool insert;

    TConsole(uint8_t ww = 80, uint8_t hh = 25) : w(ww), h(hh), x(0), y(0),bg(0),fg(7) {
		 insert = 0;
       
        hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        hStdIn = GetStdHandle(STD_INPUT_HANDLE);
        GetConsoleMode(hStdIn, &saveScreenConsoleMode);

		GetConsoleCursorInfo(hStdOut,&savecurosrinfo);

        CONSOLE_SCREEN_BUFFER_INFO inf;
        GetConsoleScreenBufferInfo(hStdOut, &inf);
        saveScreenWidth = inf.dwSize.X;
        saveScreenHeight = inf.dwSize.Y;
       

        hcsb = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        SetConsoleActiveScreenBuffer(hcsb);
        //SetConsoleMode(hStdIn, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
        SetConsoleMode(hStdIn, 0);

        COORD max = GetLargestConsoleWindowSize(hcsb);
        CONSOLE_SCREEN_BUFFER_INFO info2;
        GetConsoleScreenBufferInfo(hcsb, &info2);


        COORD newSize = { w, h };
        SMALL_RECT r = { 0, 0, newSize.X - 1, newSize.Y - 1 };

        SetConsoleScreenBufferSize(hcsb, newSize);
        SetConsoleWindowInfo(hcsb, TRUE, &r);
        SetConsoleScreenBufferSize(hcsb, newSize);		

//		HWND  hWnd = GetActiveWindow();
//		SetWindowText(hWnd,"hello");
		SetConsoleTitle((LPSTR)"Hello");
    }

    int KbHit(void) {
        DWORD cEvents;
        INPUT_RECORD inpRec;

        while (PeekConsoleInput(hStdIn, &inpRec, 1, &cEvents) && cEvents == 1) {
            if (inpRec.EventType != KEY_EVENT) {
                ReadConsoleInput(hStdIn, &inpRec, 1, &cEvents);
                continue;
            }
            if (!inpRec.Event.KeyEvent.bKeyDown) ReadConsoleInput(hStdIn, &inpRec, 1, &cEvents);
            else return 1;
        }
        return 0;
    }

	void GetKeyEvent(TEvent &e){
		DWORD cEvents;
		INPUT_RECORD inpRec;
		PeekConsoleInput(hStdIn, &inpRec, 1, &cEvents);
		if(cEvents == 1){
			ReadConsoleInput(hStdIn, &inpRec, 1, &cEvents);
			if (inpRec.EventType == KEY_EVENT &&
				inpRec.Event.KeyEvent.bKeyDown == 1) {
					e.charcode = inpRec.Event.KeyEvent.uChar.AsciiChar;
					e.keycode = (uint8_t)inpRec.Event.KeyEvent.wVirtualKeyCode;
			}
		}
	}

    void SetCrtModeRes(unsigned w, unsigned h) {
        CONSOLE_SCREEN_BUFFER_INFO info;

        if (!GetConsoleScreenBufferInfo(hStdOut, &info)) return ;

        if (info.dwSize.X == (int)w && info.dwSize.Y == (int)h) return;

        COORD max = GetLargestConsoleWindowSize(hStdOut);
        COORD newSize = { w, h };
        if (newSize.X>max.X) newSize.X = max.X;
        if (newSize.Y>max.Y) newSize.Y = max.Y;


        COORD newBufSize = newSize;
        if (info.dwMaximumWindowSize.X>newBufSize.X) newBufSize.X = info.dwMaximumWindowSize.X;
        if (info.dwMaximumWindowSize.Y>newBufSize.Y) newBufSize.Y = info.dwMaximumWindowSize.Y;

        if (!SetConsoleScreenBufferSize(hStdOut, newBufSize)) return ;

        SMALL_RECT r = { 0, 0, newSize.X - 1, newSize.Y - 1 };
        if (!SetConsoleWindowInfo(hStdOut, TRUE, &r)) {
            newSize.X = info.dwMaximumWindowSize.X;
            newSize.Y = info.dwMaximumWindowSize.Y;
            SetConsoleScreenBufferSize(hStdOut, newSize);
            return;
        }

        SetConsoleScreenBufferSize(hStdOut, newSize);     
    }

    ~TConsole(){
        SetCrtModeRes(saveScreenWidth, saveScreenHeight);
       SetConsoleActiveScreenBuffer(hStdOut);
       SetConsoleMode(hStdIn, saveScreenConsoleMode);
	   SetConsoleCursorInfo(hStdOut,&savecurosrinfo);
    }

	void SetCursorMode(bool _insert = false){
		insert = _insert;
	}
    void ShowCursor(bool show = true) {
		CONSOLE_CURSOR_INFO curinfo = { insert? 100:10, show };
        SetConsoleCursorInfo(hcsb, &curinfo);
    }

    void Clear(){
        DWORD written;
        COORD dwBufferCoord = { 0, 0 };      
		FillConsoleOutputCharacter(hcsb,' ', w*h, dwBufferCoord,&written);
        FillConsoleOutputAttribute(hcsb, (bg << 4) + fg, w * h, dwBufferCoord, &written);
    }

    void SetCurosrPos(uint8_t xx, uint8_t yy){
        x = xx; y = yy;
       // COORD cp = { x, y };
        //SetConsoleCursorPosition(hcsb, cp);
    }

	/*void RestoreCursor(){
		COORD cp = { vx, vy };
        SetConsoleCursorPosition(hcsb, cp);
	}*/

	void SetVisCurosrPos(uint8_t xx, uint8_t yy){
        vx = xx; vy = yy;     
    }
	void RestoreCursor(){
		   COORD cp = { vx, vy };
        SetConsoleCursorPosition(hcsb, cp);
	}

    void SetColor(uint8_t bb, uint8_t ff) {
        fg = ff;
        bg = bb;
    }
    void SetColor(uint8_t attr){
        bg = attr >> 4;
        fg = attr & 0x0f;
    }

	void WriteAlign(char *t,uint8_t length, align a = center){
		char text[255+1];
		memset(text,' ',length);
		text[length] = 0;

		int len = strlen(t);
		int coffset = (int)(length-len)/2;
		/*
		int i,w = GetWidth();
		for(i= 0;i<w;i++){
			if(i<(w-len)/2) text[i] = ' ';
			else if(i>=(len+(w-len)/2))text[i] = ' ';
			else text[i] = *p++;
		}
		text[i] = 0;
		*/
		switch(a){			
			case right:
				strcpy(text+length-len,t);
				break;
			case center:
				strcpy(text+coffset,t);
				if((coffset+len)<length) text[coffset+len] = ' ';
				break;
			case left:
			default:
				strcpy(text,t);
				if(len<length) text[len] = ' ';					 
				break;
		}



		Write(text);		
	}

    void Write(char *t) {

        int len = strlen(t);

        COORD dwBufferCoord = { 0,0 };
        SMALL_RECT rcWriteRegion = { x,y, w, h };
        COORD dwBufferSize = { len,1 };

        
        CHAR_INFO data[255];
        for (int i = 0; i < len; i++) {
            data[i].Char.AsciiChar = *t++;
            data[i].Attributes = (bg << 4) + fg;
        }
        
        WriteConsoleOutput(hcsb, data, dwBufferSize, dwBufferCoord, &rcWriteRegion);
        
		//Sleep(4);

    }

    void Write(char *t, char *a) {

        int len = strlen(t);

        COORD dwBufferCoord = { 0, 0 };
        SMALL_RECT rcWriteRegion = { x, y, w, h };
        COORD dwBufferSize = { len, 1 };

        CHAR_INFO data[255];
        for (int i = 0; i < len; i++) {
            data[i].Char.AsciiChar = *t++;
            data[i].Attributes = *a++;
        }

        WriteConsoleOutput(hcsb, data, dwBufferSize, dwBufferCoord, &rcWriteRegion);


    }

};
