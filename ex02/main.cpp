#include "PmergeMe.hpp"

template <typename T>
void print_numbers(const T& cont)
{
	for (size_t i = 0; i < cont.size(); ++i)
	{
		std::cout << cont[i];
		if (i + 1 < cont.size())
			std::cout << " ";
	}
	std::cout << std::endl;
}

int main(int ac, char **av)
{
	PmergeMe a;
	try
	{
		a.parse_parameters(ac, av);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return (1);
	}
	std::cout << "Before:\t";
	print_numbers(a.get_vect());
	std::clock_t vect_start = std::clock();
	a.vect_sort();
	std::clock_t vect_end = std::clock();
	// std::clock_t deq_start = std::clock();
	// a.deq_sort();
	// std::clock_t deq_end = std::clock();
	std::cout << "after:\t";
	print_numbers(a.get_vect());
	double durationVec = 1000000.0 * (double)(vect_end - vect_start) / CLOCKS_PER_SEC;
	std::cout << "Time to process a range of " << a.get_vect().size()
				<< " elements with std::vector : " << durationVec << " us" << std::endl;

	// double durationDeque = 1000000.0 * (double)(deq_end - deq_start) / CLOCKS_PER_SEC;
	// std::cout << "Time to process a range of " << a.get_vect().size()
	// 			<< " elements with std::deque : " << durationDeque << " us" << std::endl;
}
