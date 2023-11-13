#pragma once
#include "stdint.h"

struct TPoint {
	uint8_t x, y;
};

struct TEvent{
	uint8_t keycode;
	uint8_t shift;
	uint8_t charcode;
};

struct TObject;
typedef int (*do_something)(TObject *p); 

#pragma pack(1) // affects derived classes also
struct TObject {
private:
	int16_t offset;

public:
	TObject* Next() const { return offset != 0 ? (TObject*)((uint8_t*)this-offset) : 0;}
	TObject() : offset(0) {}
	~TObject(){};

	TObject& Add(TObject &o){
		TObject *p = this;
		while (p->Next()) p = p->Next();
		p->offset = (uint8_t*)p - (uint8_t*)(&o);
		return *this;
	}

	uint8_t GetNoItems(){
		uint8_t i = 0;
		TObject *p = this;
		while (p) {
			i++;
			p = p->Next();
		}
		return i;
	}

	TObject* GetAt(uint8_t i){
		TObject *p = this;
		while (p){
			if (i-- == 0) return p;
			p = p->Next();
		}
		return this;
	}

	void ForEach( do_something f){
		TObject *p = this;
		while (p){
			if((*f)(p) == 0) break;
			p = p->Next();
		}
	}	
};

inline TObject& operator + (TObject& s1, TObject& s2) { return s1.Add(s2); }
