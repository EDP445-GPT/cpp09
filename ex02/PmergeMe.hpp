#pragma once


#include <iostream>
#include <vector>
#include <deque>
#include <exception>
#include <ctime>
#include <cstdlib>


class PmergeMe
{
	private :
		std::vector<int> vect;
		std::deque<int> deq;
		size_t cmp;
	public:
		PmergeMe();
		PmergeMe(const PmergeMe &obj);
		PmergeMe &operator=(const PmergeMe &obj);
		~PmergeMe();
		void parse_parameters(int ac, char **av);
		const std::vector<int> &get_vect();
		const std::deque<int> &get_deq();
		void vect_sort();
		void deq_sort();
};
