/*if(!check_dig(operating, vars)){
	int jump_in = take_address_empty_mem();
	as.emplace_back(make_zero(num_of_operation) + " JUMP " + make_zero(jump_in));
	operating = change_variable_to_mem(RPN(operating), vars);
	cout << remove_spaces(operating) << endl;
	vector<string> memory = parsing_polish(remove_spaces(operating), jump_in);
	print_vector(memory);
	make_operations(as, memory, remove_spaces(operating), jump_in);
}*/
void make_operations(vector<string> &as, vector<string> mem, string str, int &num_of_operation){
  int mem_num = 0;
  for(int i = 0; i < str.length(); ++i){
    if(str[i] == '+'){
      make_note(as, " LOAD ", num_of_operation, mem_num, mem);
      make_note(as, " ADD ", num_of_operation, mem_num, mem);
      make_note(as, " STORE ", num_of_operation, mem_num, mem);
    }
    if(str[i] == '-'){
      make_note(as, " LOAD ", num_of_operation, mem_num, mem);
      make_note(as, " SUB ", num_of_operation, mem_num, mem);
      make_note(as, " STORE ", num_of_operation, mem_num, mem);
    }
    if(str[i] == '*'){
      make_note(as, " LOAD ", num_of_operation, mem_num, mem);
      make_note(as, " MUL ", num_of_operation, mem_num, mem);
      make_note(as, " STORE ", num_of_operation, mem_num, mem);
    }
    if(str[i] == '/'){
      make_note(as, " LOAD ", num_of_operation, mem_num, mem);
      make_note(as, " DIVIDE ", num_of_operation, mem_num, mem);
      make_note(as, " STORE ", num_of_operation, mem_num, mem);
    }
  }
}

void make_note(vector<string> &as, string operation, int &num_of_operation, int& mem_num, vector<string> mem){
  as.emplace_back(make_zero(num_of_operation) + operation + mem[mem_num]);
  num_of_operation++;
  mem_num++;
}
vector<string> parsing_polish(string str, int num_of_operation){
  int step = 0;
  vector<string> memory;
  int count_oper = 0;
  for(int i = 0; i < str.length(); ++i)
    if(str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/')
      count_oper++;
  count_oper *= 3;
  for(int i = num_of_operation; i < num_of_operation + count_oper + 1; ++i)
    take_site_mem(i);
  for(int i = 0; i < str.length(); ++i){
    if(str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/'){
      cout << str << endl;
      str.erase(i,1);
      string operand_1 = str.substr(i - 7, 2);
      if(operand_1 == "_0"){
        operand_1 = make_zero(take_empty_address());
        step++;
      }
      cout << operand_1 << endl;
      memory.emplace_back(operand_1);
      string operand_2 = str.substr(i - 3, 2);
      cout << operand_2 << endl;
      memory.emplace_back(operand_2);
      string new_mem = make_zero(take_address_empty_mem());
      cout << new_mem << endl;
      memory.emplace_back(new_mem);
      str.replace(i - 7 + step, 6 - step, new_mem);
      cout << str << endl;
      i -= 7 + step;
      step = 0;
    }
  }
  memory.emplace_back(str.substr(1, 2));
  return memory;
}


int take_empty_address(){
  int address = 0;
  int value = 1;
  while(value != 0){
    s.sc_memoryGet(address, &value);
    address++;
  }
  return address - 1;
}

/*
string change_variable_to_mem(string str, vector<variable> vars){
  for(int i = 0; i < str.length(); ++i){
      if(str[i] == '0'){
        str.replace(i, 1, "_0_");
        i += 3;
    }
  }
  for(int i = 0; i < str.length(); ++i){
    for(int j = 0; j < vars.size(); ++j){
      if(str[i] == vars[j].name_variable[0])
      {
        str.replace(i, 1, "_" + make_zero(vars[j].num_of_mem) + "_");
      }
    }
  }
  return str;
}*/

int find_last_mem(){
  int address = 0;
  int value = 1;
  while(value != 0){
    s.sc_memoryGet(address, &value);
    address++;
  }
  return address - 3;
}

bool check_creature(string &a, vector<variable> variables, vector<int> &memory){
  int var_num = 0;
  for(int i = 0; i < a.length(); ++i){
    for(int j = 0; j < variables.size(); ++j){
      if(variables[j].name_variable[0] == a[i]){
        memory.emplace_back(variables[j].num_of_mem);
        var_num++;

      }
    }
  }
  if(var_num == 1){
    return false;
  }
  if(var_num > 1){
    return true;
  }
}

void print_vector_struct(vector<variable> a){
  for(int i = 0; i < a.size(); ++i)
    cout << a[i].num_of_mem << " " << a[i].name_variable << std::endl;
}
