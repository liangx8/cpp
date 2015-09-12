#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <libintl.h>
#include "life_pattern.h"

#define PP 5

LifePattern::~LifePattern(){
	delete [] matrix;
	delete [] parent;
	SelectObject(_hdcMem,_hbmOld);
	DeleteObject(_hbmMem);
	DeleteDC(_hdcMem);
	std::cout << gettext("release memory") << std::endl;
}
LifePattern::LifePattern(int size,HWND hwnd){
	matrix=new int[size*size];
	parent=new int[size*size];
	_size=size;
	HDC hdc=GetDC(hwnd);
	_hdcMem=CreateCompatibleDC(hdc);
	_hbmMem=CreateCompatibleBitmap(hdc,_size*PP,_size*PP);
	_hbmOld=(HBITMAP)SelectObject(_hdcMem,_hbmMem);
	
	ReleaseDC(hwnd,hdc);
	
}

void LifePattern::new_life(int x,int y) {
	x = (x-10)/PP;
	y = (y-10)/PP;
	std::cout << x << ',' << y << std::endl;
	if(x>=_size)return;
	if(y>=_size)return;
	matrix[x+y*_size]=1;
	paint2dcBuffer();
}
bool LifePattern::has_life(int x,int y) const{
	if(x<0) return false;
	if(y<0) return false;
	if(x>=_size) return false;
	if(y>=_size) return false;
	return matrix[x+y*_size];
}
void LifePattern::next(void){
	for(int y=0;y<_size;y++){
		for(int x=0;x<_size;x++){
			int count=0;
			if(has_life(x-1,y-1)) count ++;
			if(has_life(x-1,y)) count ++;
			if(has_life(x-1,y+1)) count ++;
			if(has_life(x,y-1)) count ++;
			if(has_life(x,y+1)) count ++;
			if(has_life(x+1,y-1)) count ++;
			if(has_life(x+1,y)) count ++;
			if(has_life(x+1,y+1)) count ++;
			switch(count){
				case 0:
				case 1:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
					parent[x+y*_size]=0;
					break;
				case 3:
					parent[x+y*_size]=1;
					break;
				case 2:
					parent[x+y*_size]=matrix[x+y*_size];
					break;
				default:
					std::cerr << "error" << std::endl;
			}
		}
	}
	for(int i=0;i<_size*_size;i++){
		matrix[i]=parent[i];
	}
	paint2dcBuffer();

}
void LifePattern::paint2dcBuffer(void){
// draw to buffer
	RECT rect;
	rect.left=10;
	rect.top=10;
	rect.right=PP*_size+10;
	rect.bottom=PP*_size+10;
	
	// double buffering technology
	
//	HBRUSH hbBg=CreateSolidBrush(RGB(240,230,220));
	HBRUSH hbBg=(HBRUSH)(COLOR_WINDOW+2);
	FillRect(_hdcMem,&rect,hbBg);

	HBRUSH hbFg=CreateSolidBrush(RGB(245,136,78));
	HBRUSH hb;
	for(int y=0;y<_size;y++){
		for(int x=0;x<_size;x++){
			if(matrix[y*_size+x]){
				hb=hbFg;
			}else{
				hb=hbBg;
			}
			rect.left	= x*PP;
			rect.top	= y*PP;
			rect.right	= PP+x*PP;
			rect.bottom	= PP+y*PP;
			FillRect(_hdcMem,&rect,hb);
		}
	}
	DeleteObject(hbFg);
//	DeleteObject(hbBg);
}
void LifePattern::paint(HDC hScreen) const{
/*
	RECT rect;
	rect.left=10;
	rect.top=10;
	rect.right=PP*_size+10;
	rect.bottom=PP*_size+10;
	
	// double buffering technology
	HDC hdcBuf=CreateCompatibleDC(hScreen);
	HBITMAP hbm= CreateCompatibleBitmap(hScreen,_size*PP,_size*PP);
	SelectObject(hdcBuf,hbm);
	
//	HBRUSH hbBg=CreateSolidBrush(RGB(240,230,220));
	HBRUSH hbBg=(HBRUSH)(COLOR_WINDOW+2);
	FillRect(hdcBuf,&rect,hbBg);

	HBRUSH hbFg=CreateSolidBrush(RGB(245,136,78));
	HBRUSH hb;
	for(int y=0;y<_size;y++){
		for(int x=0;x<_size;x++){
			if(matrix[y*_size+x]){
				hb=hbFg;
			}else{
				hb=hbBg;
			}
			rect.left	= x*PP;
			rect.top	= y*PP;
			rect.right	= PP+x*PP;
			rect.bottom	= PP+y*PP;
			FillRect(hdcBuf,&rect,hb);
		}
	}
	DeleteObject(hbFg);
	DeleteObject(hdcBuf);
	DeleteObject(hbm);
//	DeleteObject(hbBg);
*/
	BitBlt(hScreen,10,10,_size*PP,_size*PP,_hdcMem,0,0,SRCCOPY);
//	std::cout << "paint" << std::endl;
}
void LifePattern::init(void){
	srand(time(NULL));
	for(int i=0;i<_size*_size;i++){
		int v=rand() %100;
		if(v>80) 
			matrix[i]=1;
		else
			matrix[i]=0;
	}
	paint2dcBuffer();
}
