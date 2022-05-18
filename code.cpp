#include <iomanip>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include "mySimpleComputer.h"
#include "myTerm.h"
#include "myBigChars.h"
#include "myReadKey.h"
#include "disk.h"

enum commands{
	READ = 10,
	WRITE = 11,
	LOAD = 20,
	STORE = 21,
	ADD = 30,
	SUB = 31,
	DIVIDE = 32,
	MUL = 33,
	JUMP = 40,
	JNEG = 41,
	JZ = 42,
	HALT = 43,
	SUBC = 66,
};

int size_number(int num){
	if(num == 0)
	return 3;
	else if(num / 16 == 0)
	return 3;
	else if(num / 256 == 0)
	return 2;
	else if(num / 4096 == 0)
	return 1;
	else
	return 0;
}

bool check_negative(int a){
	if(a >> 14 == 1) return 1;
	else return 0;
}

int convert(int a){
	int num = -a;
	for(int i = 0; i < 15; ++i)
		num ^= 1 << i;
	return num + 1;
}
int reconvert(int a){
	int num = a - 1;
	for(int i = 0; i < 15; ++i)
		num ^= 1 << i;
		return -num;
}

void print_memory(sc s, mt b, int i, int j, colors bg, colors fg){
	int value;
	b.mt_gotoXY(2, 2);
	for(int j = 0; j < 10; ++j){
		for(int i = 0; i < 10; ++i){
			s.sc_memoryGet(i + j*10, &value);
			b.mt_gotoXY(2 + 6*i, 2 + 2*j);
			if(check_negative(value) == 0)
				std::cout << "+" << std::hex << std::setw(4) << std::setfill('0') <<
				std::hex << value << std::endl;
			else
				std::cout << std::hex << std::setw(4) << std::setfill('0') <<  std::hex
				<< value << std::endl;
		}
	}
	b.mt_setbgcolor(bg);
	b.mt_setfgcolor(fg);
	s.sc_memoryGet(i + j*10, &value);
	b.mt_gotoXY(2 + 6*i, 2 + 2*j);
	if(check_negative(value) == 0)
		std::cout << "+" << std::setw(4) << std::setfill('0') << std::hex << value << std::endl;
	else
		std::cout << std::setw(4) << std::setfill('0') << std::hex<< value << std::endl;
}

void print_big_char(int num,bc a){
	int i;
	if(check_negative(num) == 0){
		a.bc_printbigchar(a.bcintp, 2, 23, _default, _default);
		i = 11;
	}
	else{
		i = 2;
	}

	std::stringstream stream;
	stream << std::hex << num;
	std::string result(stream.str());
	int n = 4 - result.length();
	while(n != 0){
		a.bc_printbigchar(a.bcint0, i, 23, _default, _default);
		i += 9;
		n -= 1;
	}
	for(int j = 0; j < result.length(); ++j){
		if(result[j] == '0'){
			a.bc_printbigchar(a.bcint0, i, 23, _default, _default);
		}
		if(result[j] == '1'){
			a.bc_printbigchar(a.bcint1, i, 23, _default, _default);
		}
		if(result[j] == '2'){
			a.bc_printbigchar(a.bcint2, i, 23, _default, _default);
		}
		if(result[j] == '3'){
			a.bc_printbigchar(a.bcint3, i, 23, _default, _default);
		}
		if(result[j] == '4'){
			a.bc_printbigchar(a.bcint4, i, 23, _default, _default);
		}
		if(result[j] == '5'){
			a.bc_printbigchar(a.bcint5, i, 23, _default, _default);
		}
		if(result[j] == '6'){
			a.bc_printbigchar(a.bcint6, i, 23, _default, _default);
		}
		if(result[j] == '7'){
			a.bc_printbigchar(a.bcint7, i, 23, _default, _default);
		}
		if(result[j] == '8'){
			a.bc_printbigchar(a.bcint8, i, 23, _default, _default);
		}
		if(result[j] == '9'){
			a.bc_printbigchar(a.bcint9, i, 23, _default, _default);
		}
		if(result[j] == 'a'){
			a.bc_printbigchar(a.bcintA, i, 23, _default, _default);
		}
		if(result[j] == 'b'){
			a.bc_printbigchar(a.bcintB, i, 23, _default, _default);
		}
		if(result[j] == 'c'){
			a.bc_printbigchar(a.bcintC, i, 23, _default, _default);
		}
		if(result[j] == 'd'){
			a.bc_printbigchar(a.bcintD, i, 23, _default, _default);
		}
		if(result[j] == 'e'){
			a.bc_printbigchar(a.bcintE, i, 23, _default, _default);
		}
		if(result[j] == 'f'){
			a.bc_printbigchar(a.bcintF, i, 23, _default, _default);
		}
		i += 9;
		num = num / 16;
	}

}

void boxes(int x1, int y1, int x2, int y2, int goto_x1, int goto_y1, const char* message, mt b, bc a){
	a.bc_box(x1, y1, x2, y2);
	b.mt_gotoXY(goto_x1, goto_y1);
	b.mt_setbgcolor(white);
	b.mt_setfgcolor(black);
	std::cout << message;
	b.mt_setbgcolor(_default);
	b.mt_gotoXY(61,22);
}

void print_keys(mt b, int y, const char* message){
	b.mt_gotoXY(48,y);
	std::cout << message;
}

int num_incstruct = 0;
sc s;
mt b;
bc a;
int width;
int height;
int command_operand;
int num_accum = 0;
int key = 0;
int key_temp = 0;
rk r;
std::string output = "";
void Operation(){
	s.sc_memoryGet(width + height*10, &command_operand);
	int command;
	int operand;
	s.sc_commandDecode(command_operand, &command, &operand);
	b.mt_gotoXY(67, 14);
	b.mt_setbgcolor(_default);
	b.mt_setfgcolor(white);
	if(command_operand == 0)
	std::cout << "+00 : 00";
	else{
		std::cout << "+";
		for(int i = 0; i < size_number(command) - 2; ++i)
		std::cout << "0";
		std::cout << command;
		std::cout << " : ";
		for(int i = 0; i < size_number(operand) - 2; ++i)
		std::cout << "0";
		std::cout << operand;
	}
}
void Flags(){
	int flag;
	s.sc_regGet(p, &flag);
	if(flag == 1){
		b.mt_gotoXY(67, 19);
		std::cout << "P";
	}
	s.sc_regGet(o, &flag);
	if(flag == 1){
		b.mt_gotoXY(69, 19);
		std::cout << "O";
	}
	s.sc_regGet(m, &flag);
	if(flag == 1){
		b.mt_gotoXY(71, 19);
		std::cout << "M";
		num_accum = 0;
	}
	s.sc_regGet(e, &flag);
	if(flag == 1){
		b.mt_gotoXY(73, 19);
		std::cout << "E";
	}
	s.sc_regGet(t, &flag);
	if(flag == 1){
		b.mt_gotoXY(75, 19);
		std::cout << "T";
	}
}

void Big_char(){
	int value;
	print_memory(s, b, width, height, white, black);
	s.sc_memoryGet(width + height*10, &value);
	if(value >= 0)
		print_big_char((value), a);
	else
		print_big_char((value), a);
}
void accumulator(){
	if(!check_negative(num_accum) && num_accum > 16383 || check_negative(num_accum) && reconvert(num_accum) < -16383){
		num_accum = 0;
		s.sc_regSet(p,1);
	}
	b.mt_gotoXY(69, 3);
	b.mt_setbgcolor(_default);
	b.mt_setfgcolor(white);
	if(check_negative(num_accum) == 1){
		for(int i = 0; i < size_number(num_accum); ++i){
			std::cout << "0";
		}
		std::cout << std::hex << num_accum;
	}
	else{
	std::cout << "+";
	for(int i = 0; i < size_number(num_accum); ++i){
		std::cout << "0";
	}
	std::cout << std::hex << num_accum;
}

}

void instructionCounter(){
	b.mt_gotoXY(69, 9);
	b.mt_setbgcolor(_default);
	b.mt_setfgcolor(white);
	std::cout << "+";
	for(int i = 0; i < size_number(num_incstruct); ++i){
		std::cout << "0";
	}
	std::cout << std::hex << num_incstruct;

}
void form_main_screen(){

	b.mt_clrscr();
	boxes(1,1,61,21,30,1,"Memory", b,a);
	boxes(62,1,81,5,66,1,"accumulator",b,a);
	boxes(62,7,81,11,63,7,"instructionCounter",b,a);
	boxes(62,12,81,16,67,12,"Operation",b,a);
	boxes(62,17,81,21,69,17,"Flags",b,a);
	a.bc_box(1, 22, 46, 31);
	boxes(47,22,81,31,48,22,"Keys:", b, a);

	const char* messages[] = {"l  - load", "s  - save", "r  - run", "t  - step",
	"i  - reset", "F5  - accumulator", "F6  - instructionCounter"};
	for(int i = 23; i < 30; ++i)
	print_keys(b,i, messages[i - 23]);



	// Operation
	Operation();
	//Flags
	Flags();
	// Big char number
	Big_char();
	// accumulator
	accumulator();
	// instructionCounter
	instructionCounter();


	b.mt_gotoXY(0,32);
	std::cout << "Input:" << std::endl;
	std::cout << output << std::endl;
	b.mt_gotoXY(10,33);






}

char* change_current(rk r){
	char *buf = new char[50];
	read(0,buf,50);
	return buf;
}


void main_loop();


int ALU(int command, int operand){
	int value;
	if(check_negative(num_accum)){
		num_accum = reconvert(num_accum);
	}
	if(check_negative(operand)){
		operand = reconvert(operand);
	}

	if(command == ADD){
		s.sc_memoryGet(operand, &value);
		num_accum += value;
		if(!check_negative(num_accum) && num_accum > 16383 || check_negative(num_accum) && reconvert(num_accum) < -16383){
			num_accum = 0;
			s.sc_regSet(p,1);
		}
		if(num_accum < 0)
			num_accum = convert(num_accum);
	}
	if(command == SUB){
		s.sc_memoryGet(operand, &value);
		if(num_accum - value >= 0)
			num_accum -= value;
		else{
			num_accum = convert(num_accum - value);
		}
		if(!check_negative(num_accum) && num_accum > 16383 || check_negative(num_accum) && reconvert(num_accum) < -16383){
			num_accum = 0;
			s.sc_regSet(p,1);
		}
	}
	if(command == DIVIDE){
		s.sc_memoryGet(operand, &value);
			if(value == 0){
				s.sc_regSet(p, 1);
				return -1;
			}
			num_accum /= value;
		if(num_accum < 0)
			num_accum = convert(num_accum);
			if(!check_negative(num_accum) && num_accum > 16383 || check_negative(num_accum) && reconvert(num_accum) < -16383){
				num_accum = 0;
				s.sc_regSet(p,1);
			}
		}
	if(command == MUL){

		s.sc_memoryGet(operand, &value);
		num_accum *= value;
		if(num_accum < 0)
			num_accum = convert(num_accum);
			if(!check_negative(num_accum) && num_accum > 16383 || check_negative(num_accum) && reconvert(num_accum) < -16383){
				num_accum = 0;
				s.sc_regSet(p,1);
			}
	}
	if(command == SUBC){
		s.sc_memoryGet(num_accum, &value);
		if(value - operand >= 0)
			num_accum = value - operand;
		else{
			num_accum = convert(value - operand);
		}
		if(!check_negative(num_accum) && num_accum > 16383 || check_negative(num_accum) && reconvert(num_accum) < -16383){
			num_accum = 0;
			s.sc_regSet(p,1);
		}
	}
	return 0;
}

bool WasJump = 0;
bool IsHalted = 0;
int CU(void){

	int cod_command;
	s.sc_memoryGet(num_incstruct, &cod_command);
	int command, operand;
	s.sc_commandDecode(cod_command, &command, &operand);
	if(command > 29 && command < 34 || command > 50 && command < 77)
		ALU(command, operand);
	else{
		if(command == READ){
			std::string str = std::string(change_current(r));
			int num;
			std::stringstream ss;
			ss << std::hex << str;
			ss >> num;
			s.sc_memorySet(operand, num);
		}
		if(command == WRITE){
			output.clear();
			int value;
			s.sc_memoryGet(operand, &value);
			std::stringstream ss;
			ss << std::hex << value;

			ss >> output;
			ss.clear();
			int n = 4 - output.length();

			for(int i = 0; i < n; ++i){
				output.insert(output.begin(),'0');
			}
			output.insert(output.begin(),'+');
		}
		if(command == JUMP){
			num_incstruct = operand;
			width = num_incstruct % 10;
			height = num_incstruct / 10;
			WasJump = 1;}
			form_main_screen();
		if(command == JNEG && check_negative(num_accum)){
			num_incstruct = operand;
			width = num_incstruct % 10;
			height = num_incstruct / 10;
			WasJump = 1;
			form_main_screen();
		}
		if(command == JZ && !num_accum){
			num_incstruct = operand;
			width = num_incstruct % 10;
			height = num_incstruct / 10;
			WasJump = 1;
			form_main_screen();
		}
		if(command == HALT){
			IsHalted = 1;
			return 0;
		}
		if(command == LOAD){
			int value;
			s.sc_memoryGet(operand, &value);
			num_accum = value;
		}
		if(command == STORE){
			s.sc_memorySet(operand, num_accum);
		}
	}

	return 0;

}

void timer_handler(int signo){
	if(key_temp == KEY_r){

		CU();
		if(WasJump == 0)
			width++;
		else
			WasJump = 0;
		if(width > 9){
			width = 0;
			if(height > 8)
			height = 0;
			else
			height++;
		}
		num_incstruct = width + height*10;
		form_main_screen();
		int cod_command;
		s.sc_memoryGet(num_incstruct, &cod_command);
		int command, operand;
		s.sc_commandDecode(cod_command, &command, &operand);
		if(command == HALT)
			key_temp = 0;
	}

}



void main_loop(){
	struct itimerval nval, oval;
	signal(SIGALRM,timer_handler);
	nval.it_interval.tv_sec = 0;
	nval.it_interval.tv_usec = 500000;
	nval.it_value.tv_sec = 0;
	nval.it_value.tv_usec = 500000;

	s.sc_memoryInit();
	s.sc_regInit();

	r.rk_mytermregime(0,0,5,1,1);

	s.sc_commandEncode(10,10,&command_operand);

	int num = 0;

	std::stringstream ss;
	std::string str;
	form_main_screen();
	int value;
	//std::cout << "\033[?25l" << std::endl;
	s.sc_memoryGet(width + height*10, &value);
	print_big_char(value, a);
	while(true){

		num = 0;
		form_main_screen();
		s.sc_memoryGet(width + height*10, &value);
		print_big_char(value, a);
		r.rk_readkey(&key);

		if(key == KEY_right){
			IsHalted = 0;
			width++;
			if(width > 9){
				width = 0;
				if(height > 8)
				height = 0;
				else
				height++;
			}
			num_incstruct = width + height*10;
			form_main_screen();
		}
		if(key == KEY_left){
			IsHalted = 0;
			width--;
			if(width < 0)
			width++;
			num_incstruct = width + height*10;
			form_main_screen();
		}
		if(key == KEY_down){
			IsHalted = 0;
			height++;
			if(height > 9)
			height--;
			num_incstruct = width + height*10;
			form_main_screen();
		}
		if(key == KEY_up){
			IsHalted = 0;
			height--;
			if(height < 0)
			height++;
			num_incstruct = width + height*10;
			form_main_screen();
		}
		if(key == KEY_q){
			str = std::string(change_current(r));
			ss << std::hex << str;
			ss >> num;
			ss.clear();
			if(num < 0)
				num = convert(num);
				s.sc_memorySet(width + height*10, num);
			form_main_screen();
			num = 0;
		}
		if(key == KEY_s){
			char filename[] = "code.dat";
			s.sc_memorySave(filename);
		}
		if(key == KEY_l){
			char filename[] = "code.dat";
			s.sc_memoryLoad("file.o");
			form_main_screen();
		}
		if(key == KEY_i){
			s.sc_memoryInit();
			s.sc_regInit();
			form_main_screen();
		}
		if(key == KEY_f5){
			str = std::string(change_current(r));
			ss << std::hex << str;
			ss >> num_accum;
			if(num_accum < 0)
				num_accum = convert(num_accum);
			ss.clear();
			form_main_screen();
		}
		if(key == KEY_f6){
			str = std::string(change_current(r));
			ss << std::hex << str;
			ss >> num_incstruct;
			ss.clear();
			width = num_incstruct % 10;
			height = num_incstruct / 10;
			form_main_screen();
		}
		if(key == KEY_r){
			setitimer (ITIMER_REAL, &nval, &oval);
			key_temp = key;
			key = 0;
			while(key != KEY_r){
				if(IsHalted == 1)
					break;
				r.rk_readkey(&key);
			}
			signal(SIGUSR1,timer_handler);
			setitimer(ITIMER_REAL, 0, 0);
			r.rk_mytermregime(0,0,4,1,0);
			key_temp = 0;
		}
		if(key == KEY_t && !IsHalted){
				CU();
			if(WasJump == 0 && !IsHalted)
				width++;
			else
				WasJump = 0;
			if(width > 9){
				width = 0;
				if(height > 8)
				height = 0;
				else
				height++;
			}
			num_incstruct = width + height*10;
			form_main_screen();

		}
	}
}

int main(){


	main_loop();
	return 0;
}
