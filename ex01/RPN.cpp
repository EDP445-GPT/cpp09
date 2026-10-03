#include "RPN.hpp"

RPN::RPN() {}

RPN::RPN(const RPN &obj) : data(obj.data) {}

RPN &RPN::operator=(const RPN &obj)
{
	if (this != &obj)
		data = obj.data;
	return (*this);
}

RPN::~RPN() {}

static bool is_operator(char c)
{
	return (c == '+' || c == '-' || c == '*' || c == '/');
}

static long checked_add(long a, long b)
{
	if ((b > 0 && a > LONG_MAX - b) || (b < 0 && a < LONG_MIN - b))
		throw std::runtime_error("Error");
	return (a + b);
}

static long checked_sub(long a, long b)
{
	if ((b < 0 && a > LONG_MAX + b) || (b > 0 && a < LONG_MIN + b))
		throw std::runtime_error("Error");
	return (a - b);
}

static long checked_mul(long a, long b)
{
	if (a > 0)
	{
		if ((b > 0 && a > LONG_MAX / b) || (b <= 0 && b < LONG_MIN / a))
			throw std::runtime_error("Error");
	}
	else
	{
		if ((b > 0 && a < LONG_MIN / b) || (b <= 0 && a != 0 && b < LONG_MAX / a))
			throw std::runtime_error("Error");
	}
	return (a * b);
}

static long checked_div(long a, long b)
{
	if (b == 0 || (a == LONG_MIN && b == -1))
		throw std::runtime_error("Error");
	return (a / b);
}

void RPN::evaluate_expression(const std::string &exp)
{
	std::istringstream ss(exp);
	std::string token;

	data = std::stack<long>();
	while (ss >> token)
	{
		if (token.size() != 1)
			throw std::runtime_error("Error");
		if (std::isdigit(static_cast<unsigned char>(token[0])))
			data.push(token[0] - '0');
		else if (is_operator(token[0]))
		{
			if (data.size() < 2)
				throw std::runtime_error("Error");
			long right = data.top();
			data.pop();
			long left = data.top();
			data.pop();
			switch (token[0])
			{
				case '+': data.push(checked_add(left, right)); break ;
				case '-': data.push(checked_sub(left, right)); break ;
				case '*': data.push(checked_mul(left, right)); break ;
				case '/': data.push(checked_div(left, right)); break ;
			}
		}
		else
			throw std::runtime_error("Error");
	}
	if (data.size() != 1)
		throw std::runtime_error("Error");
}

long RPN::return_result() const
{
	if (data.empty())
		throw std::runtime_error("Error");
	return (data.top());
}
