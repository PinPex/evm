#include "mySimpleComputer.h"
#include <iostream>
int sc::sc_commandDecode(int value, int* command, int* operand){
	*command = value  >> 7;
	if(*command < 10 || *command > 11 && *command < 20 || *command > 21
		&& *command < 30 || *command > 33 && *command < 40 || *command > 43
		&& *command < 51 || *command > 66){
			sc_regSet(e,1);
			return -1;
		}
		else
			sc_regSet(e,0);
		*operand = value - (*command << 7);
		return 0;
	}

	int sc::sc_commandEncode(int command, int operand, int* value){
		if(command < 10 || command > 11 && command < 20 || command > 21
			&& command < 30 || command > 33 && command < 40 || command > 43
			&& command < 51)
			return -1;
			*value = (command << 7) + operand;
			return 0;
		}

		int sc::sc_regGet(int registr, int* value){
			if(registr < 1 || registr > 5){
				return -1;
			}
			*value = (flags >> (registr - 1)) & 0x1;
			return 0;
		}

		int sc::sc_regSet(int registr, int value){
			if(value == 1){
				flags |= (1 << (registr - 1));
			}
			else if(value == 0){
				flags &= ~(1 << (registr - 1));
			}
			else{
				return -1;
			}
			return 0;
		}

		int sc::sc_regInit(void){
			flags = 0;
			return 0;
		}

		int sc::sc_memoryLoad(char* filename){
			FILE* fp;
			if((fp = fopen(filename,"rb")) == NULL){
				return -1;
			}
			fread(mem,sizeof(int),100,fp);
			fclose(fp);
			return 0;
		}

		int sc::sc_memorySave(char* filename){
			FILE* fp;
			if((fp = fopen(filename,"wb")) == NULL){
				return -1;
			}
			fwrite(mem,sizeof(int),100,fp);
			fclose(fp);
			return 0;
		}

		int sc::sc_memoryGet(int address, int *value){
			if(address < 0 || address > 99){
				sc_regSet(m,1);
			}
			else{
				*value = mem[address];
			}
			return 0;

		}

		int sc::sc_memorySet(int address, int value){
			if(address < 0 || address > 99){
				sc_regSet(m, 1);
			}
			else
			mem[address] = value;
			return 0;
		}

		int sc::sc_memoryInit(){
			mem = new int[100];
			for(int i = 0; i < 100; ++i){
				mem[i] = 0;
			}
			return 0;
		}
