#include "BitcoinExchange.hpp"

int main(int ac, char **av)
{
	if (ac != 2)
		return (0);
	try
	{
		BitcoinExchange a;
		a.loadDatabase("data.csv");
		a.parser(av[1]);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}
