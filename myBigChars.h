#pragma once
#include "myTerm.h"
#include <unistd.h>
class bc{
public:
  int bcint0 [2] = {1717976064, 3958374};
  int bcint1 [2] = {1010315264, 3158064};
  int bcint2 [2] = {1010842624, 8258050};
  int bcint3 [2] = {2120252928, 8282238};
  int bcint4 [2] = {2120640000, 6316158};
  int bcint5 [2] = {1040350720, 4079680};
  int bcint6 [2] = {35789824, 1974814};
  int bcint7 [2] = {811630080, 396312};
  int bcint8 [2] = {1013332992, 3958374};
  int bcint9 [2] = {2087074816, 3956832};
  int bcintA [2] = {2118269952, 4342338};
  int bcintB [2] = {1044528640, 4080194};
  int bcintC [2] = {37895168, 3949058};
  int bcintD [2] = {1111637504, 4080194};
  int bcintE [2] = {2114092544, 8258050};
  int bcintF [2] = {33717760, 131646};
  int bcintp [2] = {2115508224, 1579134};

  int bc_printA(char* str);
  int bc_box(int x1, int y1, int x2, int y2);
  int bc_printbigchar(int mas[2], int x, int y, enum colors, enum colors);
  int bc_setbigcharpos(int *big, int x, int y, int *value);
  int bc_getbigcharpos(int *big, int x, int y, int *value);
  int bc_bigcharwrite(int fd, int *big, int count);
  int bc_bigcharread(int fd, int *big, int need_count, int *count);
};
