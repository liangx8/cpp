#ifndef GO_TRAP_H
#define GO_TRAP_H
#define MIN_SIZE 50
#define MARGIN 20
#include <windows.h>
class GoTrap{
private:
	int *_chesses;
public:
	GoTrap();
	virtual ~GoTrap();
	void paint(HDC,int) const;
	void point_clk(int,int);
};
#endif
