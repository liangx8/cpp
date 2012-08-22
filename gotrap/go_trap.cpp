#include <iostream>

#include "go_trap.h"
void drawRect(HDC h,int x1,int y1,int x2, int y2,HBRUSH hbLine){
	MoveToEx(h,x1,y1,NULL);
	LineTo(h,x1,y2);
	LineTo(h,x2,y2);
	LineTo(h,x2,y1);
	LineTo(h,x1,y1);
}

void GoTrap::paint(HDC hdc,int total_size)const{
	int grid_size=total_size/18;
	if(grid_size<30)grid_size=30;
	int size=grid_size*18;
	int layout_size=size +2*MARGIN;
	HDC hdcMem=CreateCompatibleDC(hdc);
	HBITMAP hbm=CreateCompatibleBitmap(hdc,layout_size,layout_size);
	HBRUSH hbLine=CreateSolidBrush(RGB(245,136,78));
	HBITMAP hbmOld=(HBITMAP)SelectObject(hdcMem,hbm);
	HBRUSH hb=(HBRUSH)(COLOR_WINDOW+2);

	RECT rect;
	rect.left=MARGIN;
	rect.top=MARGIN;
	rect.bottom=rect.right=MARGIN+size;
	FillRect(hdcMem,&rect,hb);
	drawRect(hdcMem,rect.left,rect.top,rect.right,rect.bottom,hbLine);
	BitBlt(hdc,MARGIN,MARGIN,layout_size,layout_size,hdcMem,0,0,SRCCOPY);

	DeleteObject(hbLine);
	DeleteObject(hbm);
	SelectObject(hdcMem,hbmOld);
	DeleteObject(hdcMem);
}
GoTrap::GoTrap(){
	_chesses=new int[19*19];
}
GoTrap::~GoTrap(){
	delete [] _chesses;
	std::cout << "release memory" << std::endl;
}