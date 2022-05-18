#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include "mySimpleComputer.h"

using namespace std;
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


template <typename T>
void print_vector(vector<T> a){
  for(int i = 0; i < a.size(); ++i)
    cout << a[i] << std::endl;
}

void remove_spaces(string &a){
  a.erase(remove(a.begin(), a.end(), ' '), a.end());
}

void cpy(vector<int> in, int* out){
  for(int i = 0; i < in.size(); ++i){
    out[i] = in[i];
  }
}

int get_address(vector<int> first){
  first.push_back(first.size());
  return first.size() - 1;
}

int main(int args, char* argv[]){
  stringstream ss;
  ifstream file(argv[1]);
  string cod_word;
  string com;
  vector<int> first;
  vector<int> command;
  vector<int> operand;

  while(getline(file,cod_word)){
    int num;
    first.emplace_back(stoi(cod_word.substr(0,2)));
    cod_word.erase(0,3);
    com = cod_word.substr(0,cod_word.find(' '));
      if(com == "READ") command.emplace_back(READ);
      if(com == "WRITE") command.emplace_back(WRITE);
      if(com == "LOAD") command.emplace_back(LOAD);
      if(com == "STORE") command.emplace_back(STORE);
      if(com == "ADD") command.emplace_back(ADD);
      if(com == "SUB") command.emplace_back(SUB);
      if(com == "DIVIDE") command.emplace_back(DIVIDE);
      if(com == "MUL") command.emplace_back(MUL);
      if(com == "JUMP") command.emplace_back(JUMP);
      if(com == "JNEG") command.emplace_back(JNEG);
      if(com == "JZ") command.emplace_back(JZ);
      if(com == "HALT") command.emplace_back(HALT);
      if(com == "SUBC") command.emplace_back(SUBC);
      if(com == "=") command.emplace_back(77);
    cod_word.erase(0,cod_word.find(' '));
    if(cod_word.find(';') != string::npos)
      com = cod_word.substr(0,cod_word.find(';'));
    else
      com = cod_word.substr(0,cod_word.length());
    remove_spaces(com);
    if(command[command.size() - 1] == 77){
      operand.emplace_back(stoul(com,nullptr,16));
    }
    else
      operand.emplace_back(stoi(com));
  }
  file.close();
  sc s;
  int code;
  FILE* out;
  if((out = fopen(argv[2],"wb")) == NULL){
    return -1;
  }
  vector<int> comms;
  for(int i = 0; i < command.size(); ++i){
    if(command[i] == 77){
      code = operand[i];
  }
    else
      s.sc_commandEncode(command[i], operand[i],&code);
    comms.emplace_back(code);
  }
  int memory[100]{0};
  for(int i = 0; i < first.size(); ++i){
    memory[first[i]] = comms[i];
  }
  fwrite(memory,sizeof(int),100,out);

  print_vector(first);
  cout << "\n";
  print_vector(command);
  cout << "\n";
  print_vector(operand);
}
