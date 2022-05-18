#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <stack>
#include <map>
#include <regex>
#include "commands.h"
#include "mySimpleComputer.h"





template <typename T>
void print_vector(vector<T> a){
  for(int i = 0; i < a.size(); ++i)
    cout << a[i] << endl;
}
void print_vector_struct(vector<variable> a){
  for(int i = 0; i < a.size(); ++i)
    cout << a[i].num_of_mem << " " << a[i].name_variable << std::endl;
}


vector<string> assembler_strings;
vector<variable> variables;
vector<stroke_mem> num_strings;

bool check_dig(string str){
  int count_var = 0;
  for(int i = 0; i < str.length(); ++i){
    for(int j = 0; j < variables.size(); ++j){
      if(str[i] == variables[j].name_variable[0])
      {
        count_var++;
      }

    }
  }
  if(count_var == 0)
    return true;
  return false;
}


string change_variable_to_mem(string str){
  str.insert(str.length() - 2, " ");
  str.insert(0, " ");
  while(str.find(' ') != string::npos){
    str.replace(str.find(' '), 1, "_");
  }

  for(int i = 0; i < str.length(); ++i){
    for(int j = 0; j < variables.size(); ++j){
      if(str[i] == variables[j].name_variable[0])
      {
        str.replace(i, 1, make_zero(variables[j].num_of_mem) + "v");
      }
    }
  }
  return str;
}


void check_operands(string operand, string operation, int &num_of_operation){
  if(operand[operand.length() - 1] == 'v'){
    operand.erase(operand.length() - 1, 1);
    assembler_strings.emplace_back(make_zero(num_of_operation) + operation + operand);
    num_of_operation++;
  }
  else{
    int num = take_address_empty_mem();
    assembler_strings.emplace_back(make_zero(num) + " = " + make_hex_mem(stoi(operand)));
    assembler_strings.emplace_back(make_zero(num_of_operation) + operation + make_zero(num));
    num_of_operation++;
  }
}




void parsing_polish(string str, int &num_of_operation){
  erase_dooble_(str);
  take_mem_for_operations(str, num_of_operation);

  str.erase(0,1);
  str.erase(str.length() - 1,1);

  vector<string> operands_oper;
  split(str,'_', operands_oper);

  for(auto i = operands_oper.begin(); i != operands_oper.end(); ++i){
    if(check((*i)[0], "+-*/")){

      check_operands(*(i - 2), " LOAD ", num_of_operation);
      check_operands(*(i - 1), operation(*i), num_of_operation);

      i = operands_oper.erase(i);
      i = operands_oper.erase(i - 1);
      i = operands_oper.erase(i - 1);

      int empt;
      if(i != operands_oper.end()){
        empt = take_address_empty_mem();
        assembler_strings.emplace_back(make_zero(num_of_operation) + " STORE " + make_zero(empt));
        num_of_operation++;
      }

      operands_oper.insert(i, make_zero(empt) + "v");

    }
  }
}



int operation(string a, int operand_1, int operand_2){
  if(a[0] == '+'){
    return operand_1 + operand_2;
  }
  if(a[0] == '-'){
    return operand_1 - operand_2;
  }
  if(a[0] == '*'){
    return operand_1 * operand_2;
  }
  if(a[0] == '/'){
    return operand_1 / operand_2;
  }
}

int parsing_polish(string str){
  str.erase(0,1);
  str.erase(str.length() - 1,1);
  vector<string> operands_oper;
  split(str,'_', operands_oper);
  print_vector(operands_oper);
  int temp;
  for(int i = 2; i < operands_oper.size(); i += 2){
    temp = operation(operands_oper[i], stoi(operands_oper[i - 2]), stoi(operands_oper[i - 1]));
    if(i == operands_oper.size() - 1)
      return temp;
    operands_oper[i] = to_string(temp);
  }
}

void check_new_element(string str){
  bool in_aphabet;
  bool in_vars;
  for(auto i : str){
    if(alphabet.find(i) != string :: npos){
      in_aphabet = true;
    }
    else
      in_aphabet = false;

    for(auto j : variables){

      if(j.name_variable[0] == i){
        in_vars = true;
        break;
      }
      else{
        in_vars = false;
      }
      }

    if(in_aphabet && !in_vars){
      string a;
      a.push_back(i);
      variable temp = {take_address_empty_mem(), a};
      variables.emplace_back(temp);
      assembler_strings.emplace_back(make_zero(temp.num_of_mem) + " = " + "+0000");
    }
  }
}

bool check_digital(string str){
  for(auto i : str)
    if(!isdigit(i))
      return false;
  return true;
}

vector<string> many_eqs(string str){
  string name_var = str.erase(0,str.find(' ') + 1);
  name_var = name_var.substr(0, name_var.rfind('=') + 1) ;
  int count_eqs = count(name_var.begin(), name_var.end(),'=');
  int count_variables = 0;

  for(auto i : name_var)
    if(alphabet.find(i) != string::npos)
      count_variables++;
  if(count_variables != count_eqs){
    cout << "Find syntax error in LET: count of varibles should equales count of equal sign" << endl;
    exit(-1);
  }
  check_new_element(name_var);
  vector<string> stores_var;
  split(name_var, '=', stores_var);
  return stores_var;
}

void Let(string str, int &num_of_operation){

  vector<string> stores_var = many_eqs(str);

  for(int i = 0; i < stores_var.size(); i++)
    stores_var[i] = remove_spaces(stores_var[i]);


  string operating = str.substr(str.rfind('=') + 1,str.length());

  if(remove_spaces(operating).empty()){
    cout << "Find syntax error in LET: after equal sign should be math" << endl;
    exit(-1);
  }

  if(check_dig(operating)){ // pure digital
    if(check_digital(remove_spaces(operating))) {
      for(auto i : stores_var){
        for(auto j : variables){
          if(i[0] == j.name_variable[0]){
            assembler_strings.emplace_back(make_zero(j.num_of_mem) + " = " + make_hex_mem(stoi(operating)));
          }
        }
      }
      assembler_strings.emplace_back(make_zero(num_of_operation) + " JUMP " + make_zero(num_of_operation + 1));
      return;
      }

    operating = change_variable_to_mem(RPN(operating));
    for(auto i : stores_var){
      for(auto j : variables)
        if(i[0] == j.name_variable[0]){
          assembler_strings.emplace_back(make_zero(j.num_of_mem) + " = " + make_hex_mem(parsing_polish(remove_spaces(operating))));
        }
    }
    assembler_strings.emplace_back(make_zero(num_of_operation) + " JUMP " + make_zero(num_of_operation + 1));
    return;

  }
  if(!check_dig(operating)){ // digital and variables
    int jump_in = take_address_empty_mem();
    assembler_strings.emplace_back(make_zero(num_of_operation) + " JUMP " + make_zero(jump_in));
    operating = remove_spaces(operating);
    if(remove_spaces(operating).length() == 1){
      for(auto i : variables)
        if(operating[0] == i.name_variable[0]){
          cout << "lol" << endl;
          operating.replace(0, 2, make_zero(i.num_of_mem));
          assembler_strings.emplace_back(make_zero(jump_in) + " LOAD " + operating);
          jump_in = take_address_empty_mem();;
          for(auto h : stores_var){
            for(auto j : variables)
              if(h[0] == j.name_variable[0]){
                cout << "lol" << endl;
                assembler_strings.emplace_back(make_zero(jump_in) + " STORE " + make_zero(j.num_of_mem));
                jump_in = take_address_empty_mem();

              }
          }
        }
        assembler_strings.emplace_back(make_zero(jump_in) + " JUMP " + make_zero(num_of_operation + 1));
      return;
    }


    assembler_strings.emplace_back(make_zero(num_of_operation) + " JUMP " + make_zero(jump_in));
    operating = change_variable_to_mem(RPN(operating));


    parsing_polish(operating, jump_in);
    for(auto i : stores_var){
      for(auto j : variables)
        if(i[0] == j.name_variable[0]){
          assembler_strings.emplace_back(make_zero(jump_in) + " STORE " + make_zero(j.num_of_mem));
          jump_in++;
        }
    }


    assembler_strings.emplace_back(make_zero(jump_in) + " JUMP " + make_zero(num_of_operation + 1));
  }
}

void neg_or_eq(vector<string> operands, bool invers, int &num_of_operation){
  if(invers){
    check_operands(operands[2], " LOAD ", num_of_operation);
    check_operands(operands[0], " SUB ", num_of_operation);
  }
  else{
    check_operands(operands[0], " LOAD ", num_of_operation);
    check_operands(operands[2], " SUB ", num_of_operation);
  }
}



void IF(string str, string stroke_num, int num_of_operation){
  int jump_in = take_address_empty_mem();
  assembler_strings.emplace_back(make_zero(num_of_operation) + " JUMP " + make_zero(jump_in));
  string GOTO = remove_spaces(str.substr(str.find("GOTO"), str.length() - 1).erase(0,5));

  if(str.find("<") != string::npos || str.find(">") != string::npos || str.find("=") == string::npos){
    for(int i = jump_in; i < jump_in + 4;++i)
      take_site_mem(i);
  }
  if(str.find("<") != string::npos || str.find(">") != string::npos || str.find("=") != string::npos){
    for(int i = jump_in; i < jump_in + 5;++i)
      take_site_mem(i);
  }
  if(str.find("==") != string::npos){
    for(int i = jump_in; i < jump_in + 4;++i)
      take_site_mem(i);
  }

  str = str.substr(0, str.find("GOTO"));
  check_new_element(str);

  str = change_variable_to_mem(str);
  erase_dooble_(str);
  str.erase(0,1);
  str.erase(str.length() - 1, 1);

  vector<string> operands;
  split(str,'_', operands);
  if(operands[1] == "<"){
    neg_or_eq(operands, false, jump_in);
    string num_mem;
    for(auto i : num_strings){
        if(i.stroke == GOTO)
          num_mem = i.mem;
    }
    assembler_strings.emplace_back(make_zero(jump_in) + " JNEG " + num_mem);
    assembler_strings.emplace_back(make_zero(jump_in + 1) + " JUMP " + make_zero(num_of_operation + 1));
  }
  if(operands[1] == ">"){
    neg_or_eq(operands, true, jump_in);
    string num_mem;
    for(auto i : num_strings){
        if(i.stroke == GOTO)
          num_mem = i.mem;
    }
    assembler_strings.emplace_back(make_zero(jump_in) + " JNEG " + num_mem);
    assembler_strings.emplace_back(make_zero(jump_in + 1) + " JUMP " + make_zero(num_of_operation + 1));
  }
  if(operands[1] == ">="){
    neg_or_eq(operands, true, jump_in);
    string num_mem;
    for(auto i : num_strings){
        if(i.stroke == GOTO)
          num_mem = i.mem;
    }
    assembler_strings.emplace_back(make_zero(jump_in) + " JNEG " + num_mem);
    jump_in++;
    assembler_strings.emplace_back(make_zero(jump_in) + " JZ " + num_mem);
    assembler_strings.emplace_back(make_zero(jump_in + 1) + " JUMP " + make_zero(num_of_operation + 1));
  }
  if(operands[1] == "<="){
    neg_or_eq(operands, false, jump_in);
    string num_mem;
    for(auto i : num_strings){
        if(i.stroke == GOTO)
          num_mem = i.mem;
    }
    assembler_strings.emplace_back(make_zero(jump_in) + " JNEG " + num_mem);
    jump_in++;
    assembler_strings.emplace_back(make_zero(jump_in) + " JZ " + num_mem);
    assembler_strings.emplace_back(make_zero(jump_in + 1) + " JUMP " + make_zero(num_of_operation + 1));
  }
  if(operands[1] == "=="){
    neg_or_eq(operands, false, jump_in);
    string num_mem;
    for(auto i : num_strings){
        if(i.stroke == GOTO)
          num_mem = i.mem;
    }
    assembler_strings.emplace_back(make_zero(jump_in) + " JZ " + num_mem);
    assembler_strings.emplace_back(make_zero(jump_in + 1) + " JUMP " + make_zero(num_of_operation + 1));
  }
}

vector<string> read_file(char* source_path){
  ifstream file(source_path);
  string input_line;
  vector<string> command_strings;
  int i = 0;
  while(getline(file,input_line)){

    if(input_line.find("REM") == string::npos){
      erase_dooble_spaces(input_line);
      command_strings.emplace_back(input_line);
      stroke_mem temp = {command_strings[i].substr(0,command_strings[i].find(' ')), make_zero(i)};
      num_strings.emplace_back(temp);
      i++;
    }
  }
  return command_strings;
}

int main(int args, char* argv[]){


  s.sc_memoryInit();
  vector<string> command_strings = read_file(argv[1]);

  take_mem_for_commands(command_strings.size());

  for(int i = 0; i < command_strings.size(); ++i){

    string command_string = command_strings[i].erase(0,command_strings[i].find(' ') + 1).substr(0, command_strings[i].find(' '));
    if(command_string == "INPUT"){
      variable temp = {take_address_empty_mem(), remove_spaces(command_strings[i].erase(0,command_strings[i].find(' ')))};
      variables.emplace_back(temp);
      assembler_strings.emplace_back(make_zero(i) + " READ " + make_zero(temp.num_of_mem));
      assembler_strings.emplace_back(make_zero(temp.num_of_mem) + " = " + "+0000");
    }
    if(command_string == "LET"){
      Let(command_strings[i], i);
    }
    if(command_string == "IF"){
      IF(command_strings[i].erase(0,command_strings[i].find(' ') + 1), num_strings[i].stroke, i);
    }
    if(command_string == "GOTO"){
      string str = remove_spaces(command_strings[i].erase(0,command_strings[i].find(' ') + 1));
      string num;
      for(auto j : num_strings)
        if(j.stroke == str)
          num = j.mem;
      assembler_strings.emplace_back(make_zero(i) + " JUMP " + num);
    }
    if(command_string == "PRINT"){
      string str = remove_spaces(command_strings[i].erase(0,command_strings[i].find(' ') + 1));
      string num = "null";
      for(auto j : variables){
        if(str == j.name_variable)
          num = make_zero(j.num_of_mem);
      }
      if(num != "null")
        assembler_strings.emplace_back(make_zero(i) + " WRITE " + num);
      else{
        variable temp = {take_address_empty_mem(), str};
        variables.emplace_back(temp);
        assembler_strings.emplace_back(make_zero(temp.num_of_mem) + " = " + "+0000");
        assembler_strings.emplace_back(make_zero(i) + " WRITE " + make_zero(temp.num_of_mem));
      }
    }
    if(command_string == "END"){
      assembler_strings.emplace_back(make_zero(i) + " HALT 00");
    }

  }

  print_vector_struct(variables);
  print_vector(assembler_strings);
  ofstream out_file(argv[2]);
  for(auto i : assembler_strings)
    out_file << i << endl;



}
