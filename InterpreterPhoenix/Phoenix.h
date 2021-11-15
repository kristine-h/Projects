#ifndef PHOENIX_H_
#define PHOENIX_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>

struct Variable {
	std::string identifier;
	std::string type;
	void* value;
};

class Phoenix {
public:
	void ReadFile(std::string);
	~Phoenix();
private:
	void TokenizeTheLine(std::string line);
	void FindOutWhatTheTokenIs(std::vector<std::string> tokens);
	bool TokenExistsIn(std::string token, std::vector<std::string> container);
	void CreateAVariable(std::vector<std::string> tokens);
	bool IsVariableDeclarationValid(std::vector<std::string> tokens);
	bool IsInteger(std::string value);
	bool IsReal(std::string value);
	bool IsBoolean(std::string value);
	bool IsText(std::string value);
	bool IsIdentifierValid(std::string current_identifier);
	inline void AddAnIdentifier(std::vector<std::string> identifier);
	void DoOperation(std::vector<std::string> tokens);
	void DoAssignmentOperation(std::vector<std::string> tokens, int& left, int& right);
	void DoMathOperation(std::vector<std::string> tokens, int& left, int& right);
	int GetVariableIndex(std::string token, std::vector<std::string>);
	void Conditional(std::vector<std::string> tokens);
	void Loop(std::vector<std::string> loop_condition);
	bool ConditionChecker(std::vector<std::string> condition);
	void Print(std::vector<std::string> tokens);
private:
	std::ifstream in_file;
	std::vector<std::string> identifiers;
	std::vector<std::string> data_types { "integer", "real", "boolean", "text" };
	std::vector<std::string> instructions { "case", "do-until" };
	std::vector<Variable*> variables;
	std::vector<std::string> tokens; 
	std::vector<std::string> loop_body;
	int int_operand_;
	double d_operand_;
	bool case_statement_{true};
	int index_;
};

#endif // PHOENIX_H
