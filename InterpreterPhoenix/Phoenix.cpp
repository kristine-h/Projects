#include "Phoenix.h"

void Phoenix::ReadFile(std::string str) {
	in_file.open(str);
	if (!in_file.is_open()) {
		std::cout << "Unable to open the txt file.\n";
		return;
	}
	std::string line;
	while (std::getline(in_file, line)) {
		if (line == "") { continue; }
		TokenizeTheLine(line);
		FindOutWhatTheTokenIs(tokens); 
	}
	in_file.close();
}

void Phoenix::TokenizeTheLine(std::string line) {
	tokens.clear();
	if (line == "") { return; }
	if (line == "]") { case_statement_ = true; return; }
	if (case_statement_ == false) { return; }
	std::string current_token;
	char main_delimeter = ' ';
	char text_delimeter = '"';
	line.push_back(' ');
	for (int i = 0; i < line.size(); ++i) {
		if (line[i] == text_delimeter) {
			current_token.push_back(line[i]);
			do { 
				++i; 
				current_token.push_back(line[i]); 
			} 
			while ((line[i] != text_delimeter) && (i < line.size()));
		} else if (line[i] != main_delimeter) {
		  	current_token.push_back(line[i]);
		  } else { 
			  if (current_token != "") {
			  	tokens.push_back(current_token);
			  }
			   current_token.clear();
		    }
		}
}

void Phoenix::FindOutWhatTheTokenIs(std::vector<std::string> tokens) {
	if (tokens.size() == 0) { return; }
	if (TokenExistsIn(tokens[0], data_types)) {
		CreateAVariable(tokens);
	} else if (TokenExistsIn(tokens[0], identifiers)) {
		DoOperation(tokens);
      	  } else if (tokens[0] == "print") {
		Print(tokens);		  
	   } else if (tokens[0] == "case") {
		Conditional(tokens);
	   } else if (tokens[0] == "do-until") {
		   Loop(tokens);
	     } else if ((tokens.size() == 1) && tokens[0] == "]") {
		     case_statement_ = false;
	       } else {
		       std::cout << "error: \'" << tokens[0] << "\' was not declared\n";
		       exit(1);
	       }
}

bool Phoenix::TokenExistsIn(std::string token, std::vector<std::string> container) {
	for (int i = 0; i < container.size(); ++i) {
		if (token == container[i]) {
			return true;
		}
	}
	return false;
}

void Phoenix::CreateAVariable(std::vector<std::string> tokens) {
	int assignment_length = 4;
	if (!(IsVariableDeclarationValid(tokens))) {
		std::cout << "\nerror: variable declaration is not valid\n";
		exit(1);
	} else if (!(IsIdentifierValid(tokens[1]))) {
		std::cout << "\nerror: " << tokens[1] << " is not a valid identifier for a variable\n";
		exit(1);
	} else {
		AddAnIdentifier(tokens); 
		Variable* var = new Variable;
		var->identifier = tokens[1];
		var->type = tokens[0];
		if (var->type == "real") {
			var->value = new double;
			if (tokens.size() == assignment_length) {
				*(double*)var->value = std::stod(tokens[3]); 
			} 
		} else if (var->type == "integer") {
			var->value = new int;
			if (tokens.size() == assignment_length) {
				*(int*)var->value = std::stoi(tokens[3]);
			}
		} else if (var->type == "boolean") {
			var->value = new bool;
			if (tokens.size() == assignment_length) {
				if (tokens[3] == "true"  || tokens[3] == "1") { *(bool*)var->value = true; }
				if (tokens[3] == "false" || tokens[3] == "0") { *(bool*)var->value = false; }
			}
		} else if (var->type == "text") {
			var->value = new std::string;
			if (tokens.size() == assignment_length) {
				*(std::string*)var->value = tokens[3];
			}
		}
		variables.push_back(var);
	}
}

bool Phoenix::IsVariableDeclarationValid(std::vector<std::string> tokens) {
	int valid_var_declaration_length1 = 2;
	int valid_var_declaration_length2 = 4;
	if ((tokens.size() != valid_var_declaration_length1) && (tokens.size() != valid_var_declaration_length2)) {
		return false;
	}
	if (tokens.size() == valid_var_declaration_length2) {
		if (tokens[2] != "<<") {
			return false;
		} else {
	    		if ((tokens[0] == "integer") && (!IsInteger(tokens[3]))) { return false; }
			if ((tokens[0] == "real") && (!IsReal(tokens[3]))) {return false; }
			if ((tokens[0] == "text") && (!IsText(tokens[3]))) { return false; }
			if ((tokens[0] == "boolean") && (!IsBoolean(tokens[3]))) { return false; };
		  }
	}
	return true;
}

bool Phoenix::IsInteger(std::string value) {
	if ((value[0] == '-') || value[0] == '+' || ((value[0] >= '0') && (value[0] <= '9'))) {
		for (int i = 1; i < value.size(); ++i) {
			if ((value[i] >= '0') && (value[i] <= '9')) { continue; } 
			else { return false; }
		}
	}
	return true;
}

bool Phoenix::IsReal(std::string value) {
	std::regex valid_real_number("[+-]?[0-9]+(\\.[0-9]+)?([Ee][+-]?[0-9]+)?");
	if (regex_match(value, valid_real_number)) { return true; }
	return false;
}

bool Phoenix::IsBoolean(std::string value) {
	if (value == "true" || value == "false") { return true; }
	return false;
}

bool Phoenix::IsText(std::string value) {
	char quotation_mark = '"';
	int quotation_mark_count = 0;
	for (int i = 0; i < value.size(); ++i) {
		if (value[i] == quotation_mark) {
			++quotation_mark_count;
		}
	}
	if (quotation_mark_count == 2) { return true; }
	return false;
}

bool Phoenix::IsIdentifierValid(std::string current_identifier) {
	if ((current_identifier[0] >= 'a') && (current_identifier[0] <= 'z') || 
	    (current_identifier[0] >= 'A') && (current_identifier[0] <= 'Z') ||
	    (current_identifier[0] == '_') || (current_identifier[0] == '*')) { }
	else { return false; }
	for (int i = 0; i < current_identifier.size(); ++i) {
		if ((current_identifier[i] >= 'a') && (current_identifier[i] <= 'z') || 
		    (current_identifier[i] >= 'A') && (current_identifier[i] <= 'Z') || 
		    (current_identifier[i] >= '0') && (current_identifier[i] <= '9') ||
		    (current_identifier[i] == '_') && (current_identifier[i] == '*')) {
			continue;
		} else { return false; };
	}
	if ((TokenExistsIn(current_identifier, identifiers)) || 
	     (TokenExistsIn(current_identifier, data_types)) || 
	     (TokenExistsIn(current_identifier, instructions))) { 
		return false; 
	}
	return true;	
}

inline void Phoenix::AddAnIdentifier(std::vector<std::string> identifier) {
	identifiers.push_back(identifier[1]);
}

void Phoenix::DoOperation(std::vector<std::string> tokens) {
        const int assignment_length{3};
        const int operation_length{5};
	int left;
	int right;
	if ((tokens.size() == assignment_length) || (tokens.size() == operation_length)) {
		if (tokens[1] != "<<") {
			std::cout << "\nerror: expecting << after variable " << tokens[0] << std::endl;
			exit(1);
                }
		DoAssignmentOperation(tokens, left, right);
		if (tokens.size() == operation_length) {
			DoMathOperation(tokens, left, right);
		}
	} else {
		std::cout << "\nerror: unvalid operation after " << tokens[0] << std::endl;
		exit(1);
	}
}

void Phoenix::DoAssignmentOperation(std::vector<std::string> tokens, int& left, int& right) {
	left = GetVariableIndex(tokens[0], identifiers);
	if (!(TokenExistsIn(tokens[2], identifiers))) { //the token is not a literal
		if ((variables[left]->type == "integer") && (IsInteger(tokens[2]))) {
			*(int*)variables[left]->value = std::stoi(tokens[2]);
		} else if ((variables[left]->type == "real") && (IsReal(tokens[2]))) {
			*(double*)variables[left]->value = std::stod(tokens[2]);
		  } else if ((variables[left]->type == "boolean") && (IsBoolean(tokens[2]))) {
			  if (tokens[2] == "true"  || tokens[2] == "1") { *(bool*)variables[left]->value = true; }
			  if (tokens[2] == "false" || tokens[2] == "0") { *(bool*)variables[left]->value = false; }
		   } else if ((variables[left]->type == "text") && (IsText(tokens[2]))) {
			   *(std::string*)variables[left]->value = tokens[2];
		    } else { 
			    std::cout << "error: cannot do operation with " << tokens[0] << std::endl;
			    std::cout << "possible issue: space after each operand is expected." << std::endl;
			    exit(1);
		      }
	} else {
		right = GetVariableIndex(tokens[2], identifiers);
		if ((variables[left]->type == "integer") && (variables[right]->type == "integer")) {
			*(int*)variables[left]->value = *(int*)variables[right]->value;
		} else if ((variables[left]->type == "real") && (variables[right]->type == "real")) {
			*(double*)variables[left]->value = *(double*)variables[right]->value;
		  } else if((variables[left]->type == "boolean") && (variables[right]->type == "boolean")) {
			  *(bool*)variables[left]->value = *(bool*)variables[right]->value;
		    } else if ((variables[left]->type == "text") && (variables[right]->type == "text")) {
			    *(std::string*)variables[left]->value = *(std::string*)variables[right]->value;
		      } else { 
			      std::cout << "error: cannot do <<  operation with " << tokens[0] << std::endl;
			      exit(1);
		      }
	}
}

void Phoenix::DoMathOperation(std::vector<std::string> tokens, int& left, int& right) {
	if (!(TokenExistsIn(tokens[4], identifiers))) { // the token is a literal
		if ((variables[left]->type == "integer") && (IsInteger(tokens[4]))) {
			int_operand_ = std::stoi(tokens[4]);
		} else if ((variables[left]->type == "real") && (IsReal(tokens[4]))) {
			d_operand_ = std::stod(tokens[4]);
		  } else { 
			  std::cout << "error: cannot do operation " << tokens[3] << " with " << tokens[0] << std::endl;
			  exit(1);
		  }
	} else {
		right = GetVariableIndex(tokens[4], identifiers);
		if ((variables[left]->type == "integer") && (variables[right]->type == "integer")) {
			int_operand_ = *(int*)variables[right]->value;
		} else if ((variables[left]->type == "real") && (variables[right]->type == "real")) {
			d_operand_ = *(double*)variables[right]->value;
		} else { std::cout << "error: cannot do operation with " << tokens[0] << std::endl;
			exit(1);
		  }
	}
	char op = tokens[3][0];
	switch (op) {
		case '+':
			if (variables[left]->type == "integer") {
				*(int*)variables[left]->value += int_operand_;
			} else {
				*(double*)variables[left]->value += d_operand_;
			}
			break;
		case '-':
			if (variables[left]->type == "integer") {
				*(int*)variables[left]->value -= int_operand_;
			} else {
				*(double*)variables[left]->value -= d_operand_;
			}
			break;
		case '*':
			if (variables[left]->type == "integer") {
				*(int*)variables[left]->value *= int_operand_;
			} else {
				*(double*)variables[left]->value *= d_operand_;
			}
			break;
		case '/':
			if ((variables[left]->type == "integer") && (int_operand_ != 0)) {
				*(int*)variables[left]->value /= int_operand_;
			} else if ((variables[left]->type == "real") && (d_operand_ != 0)) {
				*(double*)variables[left]->value /= d_operand_;
			} else {
				std::cout << "\nerror: division by 0 is not valid\n";
				exit(1);
			}
			break;
		default: 
			{
			 std::cout << "\nerror: " << tokens[3] << " is an unvalid operation sign\n";
			 exit(1);
			 }
                }
}

int Phoenix::GetVariableIndex(std::string identifier, std::vector<std::string> identifiers) {
        for (int i = 0; i < identifiers.size(); ++i) {
                if (identifier == identifiers[i]) {
                        return i;
                }
        }
        return -1;
}

void Phoenix::Conditional(std::vector<std::string> tokens) {	
	if (tokens.size() == 3) { // conditional can only be bool variable in here
		if (tokens[2] != "[") {
			std::cout << "\nsyntax error: expecting '[' after " << tokens[1] << " in case conditional statement\n";
			exit(1);
		}
		if (!TokenExistsIn(tokens[1], identifiers)) {
			std::cout << "error: in case conditional statement " << tokens[1] << " is not declared\n";
			exit(1);
		} else {
			index_ = GetVariableIndex(tokens[1], identifiers);
		}
		if (variables[index_]->type != "boolean") {
			std::cout << "\nerror: the variable after case must be boolean\n";
			exit(1);
		}
		if (*(bool*)variables[index_]->value == false) {
			case_statement_ = false;
			return;
		}
	} else if (tokens.size() == 5) { //relational and equality operations
		if (tokens[4] != "[") {
			std::cout << "\nsyntax error: expecting '[' after " << tokens[3] << " in case conditional statement\n";
			exit(1);
		}
		case_statement_ = ConditionChecker(tokens);
	} else {
		std::cout << "\nerror in case conditional statement\n";
		exit(1);
	}
}

void Phoenix::Loop(std::vector<std::string> loop_condition) {
	if (loop_condition.size() == 5) {
		std::vector<std::string> condition;
		condition.clear();
		for (int i = 0; i < loop_condition.size(); ++i) {
			condition.push_back(tokens[i]);
		}
		if (loop_condition[4] != "[") {
			std::cout << "\nsyntax error: expecting '[' after " << loop_condition[3] << " in do-until loop\n";
			exit(1);
		}
		loop_body.clear();
		std::string line;
		while (std::getline(in_file, line)) {
			if (line == "]") { break; }
			loop_body.push_back(line);
		}
		while (ConditionChecker(condition)) {
			for (int i = 0; i < loop_body.size(); ++i){
				tokens.clear();
				TokenizeTheLine(loop_body[i]);
				FindOutWhatTheTokenIs(tokens);
			}
		}
	} else { std::cout << "\nerror in do-until length\n"; exit(1); }
}
		
bool Phoenix::ConditionChecker(std::vector<std::string> condition) {
	double operand1;
	double operand2;
	if (!(TokenExistsIn(condition[1], identifiers))) { //operand1 is a literal
		if ((IsInteger(condition[1]) || IsReal(condition[1]))) {
			operand1 = std::stod(condition[1]);
		} else { std::cout << "error: in the control structure cannot do operation " << condition[2] << std::endl;
			exit(1);
		}
	} else {
		int index = GetVariableIndex(condition[1], identifiers);
		if (variables[index]->type == "integer") {
			operand1 = *(int*)variables[index]->value;
		} else if(variables[index]->type == "real") { operand1 = *(double*)variables[index]->value; }
		else { std::cout << "error: in the control structure cannot do operation " << condition[2] <<  std::endl;
		       exit(1);
		}
	}
	if (!(TokenExistsIn(condition[3], identifiers))) { //operand2 is a literal
		if ((IsInteger(condition[3]) || IsReal(condition[3]))) {
			operand2 = std::stod(condition[3]);
		} else { std::cout << "error: in the control structure  cannot do operation " << condition[2] << std::endl;
			 exit(1);
		  }
	} else {
		int index = GetVariableIndex(condition[3], identifiers);
		if (variables[index]->type == "integer" ) {
			operand2 = *(int*)variables[index]->value;
		} else if(variables[index]->type == "real") {
			operand1 = *(double*)variables[index]->value;
		} else { std::cout << "error: in the control structure cannot do operation " << condition[2] << std::endl;
			 exit(1);
		  }
	}
	if (condition[2] == ">") { return (operand1 > operand2); }
	else if (condition[2] == "<") { return (operand1 < operand2); }
	else if (condition[2] == "=") { return (operand1 == operand2); }
	else if (condition[2] == "x=") { return (operand1 != operand2); }
	else { std::cout << "\nerror: " << condition[2] << " is an unvalid operator in the control structure\n";
		exit(1);
	}
	return false;
}

void Phoenix::Print(std::vector<std::string> tokens) {
	for (int i = 1; i < tokens.size(); ++i) {
		if ((tokens[i][0] == '\"') && (tokens[i][tokens[i].size() - 1]) == '\"') {
			tokens[i].erase(0,1);
			tokens[i].erase(tokens[i].size() - 1);
			std::cout << tokens[i] << " ";
		} else if ((tokens[i][0] != '\"')) {
			if (TokenExistsIn(tokens[i], identifiers)) {
				int j = GetVariableIndex(tokens[i], identifiers);
				if (variables[j]->type == "real") {
					std::cout << *(double*)variables[j]->value <<" ";
				} else if (variables[j]->type == "integer") {
					std::cout << *(int*)variables[j]->value << " ";
				  } else if (variables[j]->type == "text") {
					std::cout << *(std::string*)variables[j]->value << " ";
				    } else if (variables[j]->type == "boolean") {
					    std::cout << *(bool*)variables[j]->value << " ";
				    }
			} else {
				std::cout << "\nerror: '" << tokens[i] << "' was not declared.\n";
				exit(1);
			}
		}
	}
	std::cout << std::endl;
}

Phoenix::~Phoenix() {
	for (int i = 0; i < variables.size(); ++i) {
		delete variables[i];
	}
}
