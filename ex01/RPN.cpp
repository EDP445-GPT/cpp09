#include "RPN.hpp"


RPN::RPN() {}
RPN::RPN(const RPN &obj)
{
	data = obj.data;
}
RPN &RPN::operator=(const RPN &obj)
{
	if (this == &obj)
		return (*this);
	data = obj.data;
	return (*this);
}

RPN::~RPN() {}

bool is_operator(const std::string &token)
{
	if (token[0] == '+' || token[0] == '-' || token[0] == '/' || token[0] == '*')
		return true;
	return false;
}

void RPN::evaluate_expression(std::string exp)
{
	std::stringstream ss(exp);
	std::string token;

	while(ss >> token)
	{
		if (token.size() > 1)
			throw std::runtime_error("must be either a single digit number or a operator!");
		if (isdigit(token[0]))
			data.push(token[0] - '0');
		else if(is_operator(token))
		{
			if (data.size() < 2)
				throw std::runtime_error("Error: not enough operands for operator");
			int digit1, digit2;
			digit1 = data.top();
			data.pop();
			digit2 = data.top();
			data.pop();
			switch (token[0]) {
				case '+':
					data.push(digit2 + digit1);
					break;
				case '-':
					data.push(digit2 - digit1);
					break;
				case '*':
					data.push(digit1 * digit2);
					break;
				case '/':
					if (digit1 == 0) {
						throw std::runtime_error("Error: Division by zero");
					}
					data.push(digit2 / digit1);
					break;
			}
		}
		else
			throw std::runtime_error("not digit nor an operator");
	}
	if (data.size() != 1)
		throw std::runtime_error("Error: not enough operations");
}

int RPN::return_result()
{
	return (data.top());
}