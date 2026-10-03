#ifndef RPN_HPP
# define RPN_HPP

# include <iostream>
# include <stack>
# include <string>
# include <sstream>
# include <stdexcept>
# include <cctype>
# include <climits>

class RPN
{
	public:
		RPN();
		RPN(const RPN &obj);
		RPN &operator=(const RPN &obj);
		~RPN();

		void evaluate_expression(const std::string &exp);
		long return_result() const;

	private:
		std::stack<long> data;
};

#endif
