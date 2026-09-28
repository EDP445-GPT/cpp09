#pragma once

#include <iostream>
#include <stack>
#include <exception>
#include <sstream>

class RPN {
	public :
		RPN();
		RPN(const RPN &obj);
		RPN &operator=(const RPN &obj);
		~RPN();
		void evaluate_expression(std::string exp);
		int return_result();
	private:
		std::stack<int> data;
};
// "8 9 * 9 - 9 - 9 - 4 - 1 +"