#include<iostream>
#include <fstream>
#include <string>
#include<stack>
#include<iomanip>
using namespace std;


//function to check weather the character is a number
bool operandChk(char C)
{
	if (C >= '0' && C <= '9') return true;
	return false;
}
//function to check weather the character is a operator
bool chkOperator(char C)
{
	if (C == '+' || C == '-' || C == '*' || C == '/')
		return true;
	return false;
}

//function to do the operation
float doOperation(char operation, float num1, float num2)
{
	if (operation == '+') return num1 + num2;
	else if (operation == '-') return num1 - num2;
	else if (operation == '*') return num1 * num2;
	else if (operation == '/') return num1 / num2;

	else cout << "Error \n";
	return -1;
}

int oppWeight(char op) {
	switch (op) {
	case '/':return 2;
	case '*':return 2;
	case '+':return 1;
	case '-':return 1;
	default: return 0;
	}
}
//function to check weather brackets are balanced or not 
bool parenthesesChk(string exp)
{
	stack<char> temp;
	for (int i = 0; i < exp.length(); i++) {
		if (exp[i] == '(' || exp[i] == '[' || exp[i] == '{') {
			temp.push(exp[i]);
			continue;
		}
		else
		{
			if (!temp.empty()) {

				if (exp[i] == ')') {
					if (temp.top() == '(') {
						temp.pop();
						continue;
					}
					else {
						break;
					}
				}
				if (exp[i] == '}') {
					if (temp.top() == '{') {
						temp.pop();
						continue;
					}
					else {
						break;
					}
				}
				if (exp[i] == ']') {
					if (temp.top() == '[') {
						temp.pop();
						continue;
					}
					else {
						break;
					}
				}
			}
		}
	}
	if (temp.empty())
		return 1;
	else {
		return 0;
	}
}

//function to convert infix to postfix
string InfixToPostfix(string exp)
{
	char postfix[100];
	stack<char> stk;
	int count = 0;
	int i = 0,j = 0;
	int opweight;
	char character = exp[i];
	while (character != '=') {
		character = exp[i];
		if (character == '(') {
			stk.push(character);
			i++;
			continue;
		}
		if (character == ')') {
			while (!stk.empty() && stk.top() != '(') {
				postfix[j++] = stk.top();
				stk.pop();
			}
			if (!stk.empty()) {
				stk.pop();
			}
			i++;
			continue;
		}
		opweight = oppWeight(character);
		if (opweight == 0) {
			count++;
			postfix[j++] = character;
		}
		else {
			if (count > 1) {
				postfix[j++] = ' ';
			}
			if (count == 1) {
				postfix[j++] = ' ';
			}
			count = 0;
			if (stk.empty()) {
				stk.push(character);
			}
			else {
				while (!stk.empty() && stk.top() != '(' && opweight <= oppWeight(stk.top())) {
					postfix[j++] = stk.top();
					stk.pop();
				}
				stk.push(character);
			}
		}
		i++;
	}
		while (!stk.empty()) {
			postfix[j++] = stk.top();
			stk.pop();
		}
		for (int i = 0; i < 100; i++) {
			if (postfix[i] == '=') {
				postfix[i] = ' ';
			}
		}
			postfix[j] = 0;
			return postfix;
}

// Function to evaluate Postfix expression
float EvaluatePostfix(string expression)
{
	stack<float> stk;
	for (int i = 0; i< expression.length(); i++) {

		if (expression[i] == ' ' || expression[i] == ',') continue;

		else if (chkOperator(expression[i])) {
			 
			float op2 = stk.top(); stk.pop();//operand 1
			float op1 = stk.top(); stk.pop();//operand 2
			
			if (expression[i] == '/'&&op2 == 0 || op1 == 0 && op2 == 0)
			{
				return 0;
			}
			else
			{
				float result = doOperation(expression[i], op1, op2);

				stk.push(result);
			}
		}
		else if (operandChk(expression[i])) {
			float operand = 0;
			while (i<expression.length() && operandChk(expression[i])) {
				
				operand = (operand * 10) + (expression[i] - '0');
				i++;
			}
			i--;
			stk.push(operand);
		}
	}
	return stk.top();
}

//function to round the float value to two decimal places
float roundVal(float x)
{
	float val = (int)(x * 100 + 0.5);
	return (float)val / 100;
}

int main() {
	fstream newFile;
	newFile.open("formulas.txt", ios::in); //open a file
	ofstream result_file;
	result_file.open("results.txt");

	if (!newFile) 
	{
		cout << "The file cannot be loaded!!!!" << endl;
		system("pause");
	}

	if (newFile.is_open()) { //checking the file open or not
		string exp=" ";
		string expMod = " ";
		string out;
		float result;
		while (getline(newFile, exp)) { //read data from the file
		int n = exp.length();
		
		bool flag = 0;
		//check equal
		for (int i = 0; i < n; i++)
		{
			if (exp[i] == '=')
			{
				flag = 1;
			}
		}
		
		expMod = exp;
		for (int i = 0; i < n; i++)
		{
			if (expMod[i] == '[')
			{
				expMod[i] = '(';
			}
			else if (expMod[i] == ']')
			{
				expMod[i] = ')';
			}
			else if (expMod[i] == '{')
			{
				expMod[i] = '(';
			}
			else if (expMod[i] == '}')
			{
				expMod[i] = ')';
			}
			else
			{
				continue;
			}
		}

		
		if (parenthesesChk(exp) == 0 || flag == 0 || EvaluatePostfix(InfixToPostfix(expMod)) == 0)
		{
			cout << exp << " E\n";
			result_file << exp << " E\n" << endl;
		}
		else
		{
			result = EvaluatePostfix(InfixToPostfix(expMod));
			cout << exp <<roundVal(result) << endl; //display the output in the console
			result_file << exp << roundVal(result) <<endl; //export output to a file
		}
	}
		cout << endl;
		result_file.close(); //close the file
		newFile.close(); 
	}
	system("pause");
}