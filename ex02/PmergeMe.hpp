#ifndef PMERGEME_HPP
# define PMERGEME_HPP

# include <iostream>
# include <vector>
# include <deque>
# include <string>
# include <stdexcept>
# include <algorithm>
# include <cstdlib>
# include <cerrno>
# include <climits>
# include <cctype>
# include <ctime>

class PmergeMe
{
	public:
		PmergeMe();
		PmergeMe(const PmergeMe &obj);
		PmergeMe &operator=(const PmergeMe &obj);
		~PmergeMe();

		/* parses/validates argv, then sorts with both containers and times each one */
		void run(int ac, char **av);

		const std::vector<int> &get_unsorted() const;
		const std::vector<int> &get_vect() const;
		const std::deque<int> &get_deq() const;
		double get_vect_time() const;
		double get_deq_time() const;

	private:
		std::vector<int> unsorted;
		std::vector<int> vect;
		std::deque<int> deq;
		double vect_time;
		double deq_time;

		void parse_parameters(int ac, char **av);
		void vect_sort();
		void deq_sort();
};

#endif
