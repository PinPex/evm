#include "myTerm.h"

int mt::mt_clrscr(){
	std::cout << "\033[H\E[2J" << std::endl;
	return 0;
}

int mt::mt_gotoXY (int x, int y)
{
	int rows, cols;

	mt_getscreensize(&rows, &cols);
	if ((y < rows) && (x < cols) && (x >= 0) && (y >= 0))
	{
		printf("\033[%d;%dH", y, x);
		return 0;
	}
	else
	return 1;
}

int mt::mt_getscreensize(int* rows, int* cols){
	struct winsize ws;
	if(!ioctl(1, TIOCGWINSZ, &ws)){
		*rows = ws.ws_row;
		*cols = ws.ws_col;
		return 0;
	}
	else return -1;
}

int mt::mt_setfgcolor(enum colors a){
	if(a == _default){
		std::cout<< "\033[00m";
	}
	else
	std::cout << "\033[3" + std::to_string(a) + "m";
	return 0;
}

int mt::mt_setbgcolor(enum colors a){
	if(a == _default){
		std::cout<< "\033[00m";
	}
	else
	std::cout << "\033[4" + std::to_string(a) + "m";
	return 0;
}
