#pragma once

#define p 1
#define o 2
#define m 3
#define e 4
#define t 5

class sc{
public:
  int* mem;
  char flags;
  int sc_memoryInit();
  int sc_memorySet(int address, int value);
  int sc_memoryGet(int address, int *value);
  int sc_memorySave(char* filename);
  int sc_memoryLoad(char* filename);
  int sc_regInit(void);
  int sc_regSet(int registr, int value);
  int sc_regGet(int registr, int* value);
  int sc_commandEncode(int command, int operand, int* value);
  int sc_commandDecode(int value, int* command, int* operand);
};
