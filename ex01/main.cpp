#include "RPN.hpp"

int main(int ac, char **av)
{
	if (ac != 2)
		return (0);
	try
	{
		RPN a;
		a.evaluate_expression(av[1]);
		std::cout << a.return_result() << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}
