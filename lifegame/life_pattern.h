#ifndef LIFE_PATTERN
#define LIFE_PATTERN
#include <windows.h>
#define SIZE	80

class LifePattern{
private:
	int *matrix;
	int *parent;
	int _size;
	bool has_life(int x,int y) const;

	HDC _hdcMem;
	HBITMAP _hbmOld;
	HBITMAP _hbmMem;
	void paint2dcBuffer(void);
public:
	LifePattern(int,HWND);
	virtual ~LifePattern();
	void init(void);
	void paint(HDC hdc) const;
	void next(void);
	void new_life(int x,int y);
};

#endif
