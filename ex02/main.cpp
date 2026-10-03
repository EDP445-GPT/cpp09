#include "PmergeMe.hpp"

template <typename T>
static void print_numbers(const T &cont)
{
	for (std::size_t i = 0; i < cont.size(); ++i)
	{
		std::cout << cont[i];
		if (i + 1 < cont.size())
			std::cout << " ";
	}
	std::cout << std::endl;
}

int main(int ac, char **av)
{
	PmergeMe sorter;

	try
	{
		sorter.run(ac, av);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	std::cout << "Before: ";
	print_numbers(sorter.get_unsorted());
	std::cout << "After:  ";
	print_numbers(sorter.get_vect());
	std::cout << "Time to process a range of " << sorter.get_unsorted().size()
		<< " elements with std::vector : " << sorter.get_vect_time() << " us" << std::endl;
	std::cout << "Time to process a range of " << sorter.get_unsorted().size()
		<< " elements with std::deque  : " << sorter.get_deq_time() << " us" << std::endl;
	return (0);
}
