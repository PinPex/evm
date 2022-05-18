#include "myBigChars.h"

int bc::bc_printA(char *str)
{
	printf("\E(0%s\E(B", str);
	return 0;
}

int bc::bc_box(int x1, int y1, int x2, int y2)
{
	char* L = new char[1];
	L[0] = 'l';
	char* Q = new char[1];
	Q[0] = 'q';
	char* K = new char[1];
	K[0] = 'k';
	char* X = new char[1];
	X[0] = 'x';
	char* M = new char[1];
	M[0] = 'm';
	char* J = new char[1];
	J[0] = 'j';
	mt obj;
	int tmp;
	int maxx, maxy;
	int i, j;

	if (x1 > x2)
	{
		tmp = x1;
		x1 = x2;
		x2 = tmp;
	}
	if (y1 > y2)
	{
		tmp = y1;
		y1 = y2;
		y2 = tmp;
	}
	obj.mt_getscreensize(&maxy, &maxx);
	if ((x1 < 0) || (y1 < 0) || (x2 > maxx) || (y2 > maxy) || (x2 - x1 < 2) || (y2 - y1 < 2))
	return -1;
	obj.mt_gotoXY(x1, y1);
	bc_printA(L);
	for (i = x1 + 1; i < x2; i++)
	bc_printA(Q);
	bc_printA(K);
	for (i = y1 +1; i < y2; i++)
	{
		obj.mt_gotoXY(x1, i);
		bc_printA(X);
		obj.mt_gotoXY(x2, i);
		bc_printA(X);
	}
	obj.mt_gotoXY(x1, y2);
	bc_printA(M);
	for (i = x1 + 1; i < x2; i++)
	bc_printA(Q);
	bc_printA(J);
	return 0;
}

int bc::bc_printbigchar(int *big, int x, int y, enum colors fg, enum colors bg)
{
	mt obj;
	int maxx, maxy;
	int pos, bit;
	int i, j;
	char row[9];

	obj.mt_getscreensize(&maxy, &maxx);
	if ((x < 0) || (y < 0) || (x + 8 > maxx) || (y + 8 > maxy))
	return -1;
	row[8] = '\0';
	obj.mt_setfgcolor(fg);
	obj.mt_setbgcolor(bg);
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			pos = i >> 2;
			bit = (big[pos] >> ((i % 4) * 8 + j)) & 1;
			if (bit == 0)
			row[j] = ' ';
			else
			row[j] = 'a';
		}
		obj.mt_gotoXY(x, y + i);
		bc_printA(row);
	}
	obj.mt_setfgcolor(_default);
	obj.mt_setbgcolor(_default);
	return 0;
}

int bc::bc_setbigcharpos(int *big, int x, int y, int *value)
{
	int pos;

	if ((x < 0) || (y < 0)  || (x > 7) || (y > 7) || (*value < 0) || (*value > 1))
	return -1;
	if (y <3)
	pos = 0;
	else
	pos = 1;
	y = y % 4;
	if (value == 0)
	big[pos] &= ~(1 << (8 * y + x));
	else
	big[pos] |= 1 << (8 * y + x);

	return 0;
}

int bc::bc_bigcharwrite(int fd, int *big, int count)
{
	int err = write(fd, &count, sizeof(count));
	if (err == -1)
	return -1;
	err = write(fd, big, count * (sizeof(int)) * 2);
	if (err == -1)
	return -1;

	return 0;
}

int bc::bc_bigcharread(int fd, int *big, int need_count, int *count)
{
	int n, cnt, err;

	err = read(fd, &n, sizeof(n));
	if (err == -1 || (err != sizeof(n)))
	return -1;
	cnt = read(fd, big, need_count * sizeof(int) * 2);
	if (cnt == -1)
	return -1;
	*count = cnt / (sizeof(int) * 2);

	return 0;
}
