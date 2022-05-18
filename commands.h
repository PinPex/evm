#pragma once
#include "mySimpleComputer.h"
using namespace std;

struct variable{
  int num_of_mem;
  string name_variable;
};

struct stroke_mem{
  string stroke;
  string mem;
};

sc s;

string operation(string a){
  if(a[0] == '+'){
    return " ADD ";
  }
  if(a[0] == '-'){
    return " SUB ";
  }
  if(a[0] == '*'){
    return " MUL ";
  }
  if(a[0] == '/'){
    return " DIVIDE ";
  }
}


string make_hex_mem(int a){
  stringstream ss;
  string num;
  ss << hex << a;
  ss >> num;
  int n = 4 - num.length();
  string result = "+";
  for(int i = 0; i < n; ++i)
    result += "0";
  result += num;
  return result;
}

string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

vector<string> &split(const string &s, char delim, vector<string> &elems){
  stringstream ss(s);
  string item;
  while(getline(ss, item, delim)){
    elems.push_back(item);
  }
  return elems;
}

void take_site_mem(int address){
  s.sc_memorySet(address - 1 ,1);
}

string RPN(string str)
{
    string srpn;

    string::size_type ind;
    while((ind = str.find(' ')) != string::npos)
      str.erase(ind,1);

    for (int i = 0; i < str.size(); ++i) // учёт отрицательных чисел
    {
        if ((str[i] == '+' || str[i] == '-') && (0 == i || (!isalnum(str[i - 1]) && str[i - 1] != '.' && str[i - 1] != ')')))
        {
            auto it = std::find_if(str.begin() + i + 1, str.end(), [](char const c) {return !isalnum(c);});
            str.insert(it, ')');
            str.insert(i, "(0");
            int nnn = 0;
        }
    }

    std::map<char, size_t> map;
    map.insert(std::make_pair('*', 3));
    map.insert(std::make_pair('/', 3));
    map.insert(std::make_pair('+', 2));
    map.insert(std::make_pair('-', 2));
    map.insert(std::make_pair('(', 1));
    std::stack<char> stack;
    for (auto c : str) // формировка результирующей строки в ОПЗ
    {
        if (!isalnum(c) && ('.' != c))
        {
            srpn += ' ';
            if (')' == c)
            {
                while (stack.top() != '(')
                {
                    srpn += stack.top();
                    stack.pop();
                    srpn += ' ';
                }
                stack.pop();
            }
            else if ('(' == c)
            {
                stack.push(c);
            }
            else if (stack.empty() || (map[stack.top()] < map[c]))
            {
                stack.push(c);
            }
            else
            {
                do
                {
                    srpn += stack.top();
                    srpn += ' ';
                    stack.pop();
                } while (!(stack.empty() || (map[stack.top()] < map[c])));
                stack.push(c);
            }
        }
        else
        {
            srpn += c;
        }
    }
    while (!stack.empty())// остаток из стека добавляется в результ. строку
    {
        srpn += stack.top();
        srpn += ' ';
        stack.pop();
    }
    return srpn;
}

void take_mem_for_commands(int count_commands){
  for(int i = 0; i < count_commands; ++i){
    s.sc_memorySet(i,1);
  }
}

string remove_spaces(string a){
  a.erase(remove(a.begin(), a.end(), ' '), a.end());
  return a;
}

int take_address_empty_mem(){
  int address = 0;
  int value = 1;
  while(value != 0){

    s.sc_memoryGet(address, &value);
    address++;
  }
  s.sc_memorySet(address - 1, 1);
  return address - 1;
}

int take_last_not_empty(){
  int address = 0;
  int value = 1;
  while(value != 0){

    s.sc_memoryGet(address, &value);
    address++;
  }
  return address - 2;
}

string make_zero(int num){
  string res = "";
  if(num / 10 == 0){
    res = "0" + to_string(num);
  }
  else
    res = to_string(num);
  return res;
}

bool check(char chr, string alphabet){
  for(auto i : alphabet)
    if(chr == i)
      return 1;
  return 0;
}

void take_mem_for_operations(string str, int num_of_operation){
  int count_operations = 0;
  int count_var = 0;
  for(auto i : str){
    if(check(i, "+-*/"))
      count_operations++;
    if(check(i,"v"))
      count_var++;
  }
  count_operations = count_operations * 3 + 1;
  for(int i = num_of_operation; i < num_of_operation + count_operations + 1; ++i)
    take_site_mem(i);
}

void make_mem_free(int i){
	s.sc_memorySet(i, 0);
}

void erase_dooble_(string &str){
  while(str.find("__") != string::npos){
    str.replace(str.find("__"),2, "_");
  }
}

void erase_dooble_spaces(string &str){
  while(str.find("  ") != string::npos){
    str.replace(str.find("  "),2, " ");
  }
}
