#pragma once
#include <iostream>
#include <sys/ioctl.h>
enum colors{
	black,
	red,
	green,
	yellow,
	blue,
	purple,
	lblue,
	white,
	_default,
};

class mt{
public:
	int mt_clrscr(void);
	int mt_gotoXY(int x, int y);
	int mt_getscreensize(int *rows, int* cols);
	int mt_setfgcolor(enum colors);
	int mt_setbgcolor(enum colors);
};
