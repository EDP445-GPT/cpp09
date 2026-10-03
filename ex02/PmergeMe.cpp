#include "PmergeMe.hpp"

PmergeMe::PmergeMe() : vect_time(0), deq_time(0) {}

PmergeMe::PmergeMe(const PmergeMe &obj)
	: unsorted(obj.unsorted), vect(obj.vect), deq(obj.deq),
	  vect_time(obj.vect_time), deq_time(obj.deq_time) {}

PmergeMe &PmergeMe::operator=(const PmergeMe &obj)
{
	if (this != &obj)
	{
		unsorted = obj.unsorted;
		vect = obj.vect;
		deq = obj.deq;
		vect_time = obj.vect_time;
		deq_time = obj.deq_time;
	}
	return (*this);
}

PmergeMe::~PmergeMe() {}

const std::vector<int> &PmergeMe::get_unsorted() const { return (unsorted); }
const std::vector<int> &PmergeMe::get_vect() const { return (vect); }
const std::deque<int> &PmergeMe::get_deq() const { return (deq); }
double PmergeMe::get_vect_time() const { return (vect_time); }
double PmergeMe::get_deq_time() const { return (deq_time); }

/* digits only (optional leading '+'), must fit in an int */
static bool parse_positive_int(const char *s, int &out)
{
	std::string str(s);
	std::size_t i = 0;

	if (!str.empty() && str[0] == '+')
		i = 1;
	if (i >= str.size())
		return (false);
	for (std::size_t j = i; j < str.size(); j++)
		if (!std::isdigit(static_cast<unsigned char>(str[j])))
			return (false);
	errno = 0;
	long value = std::strtol(s, NULL, 10);
	if (errno == ERANGE || value > INT_MAX)
		return (false);
	out = static_cast<int>(value);
	return (true);
}

void PmergeMe::parse_parameters(int ac, char **av)
{
	if (ac < 2)
		throw std::runtime_error("Error");
	unsorted.clear();
	for (int i = 1; i < ac; i++)
	{
		int value;
		if (!parse_positive_int(av[i], value))
			throw std::runtime_error("Error");
		unsorted.push_back(value);
	}
}

// ---------------------------------------------------------------- vector
static void swap_range(std::vector<int> &vect, size_t distance, size_t AC_start, size_t BC_start)
{
	std::swap_ranges(vect.begin() + AC_start, vect.begin() + AC_start + distance, vect.begin() + BC_start);
}

static void chunk_binary_search_insert(size_t dist, std::vector<int> &wc, std::vector<int> &ls, int loser_index, int max_chunk_index)
{
	int low = 0;
	int high = max_chunk_index;
	int mid;
	int target = ls[loser_index];

	while (low <= high)
	{
		mid = low + (high - low) / 2;
		int mid_key = wc[(mid * dist) + (dist - 1)];
		if (mid_key < target)
			low = mid + 1;
		else
			high = mid - 1;
	}
	wc.insert( wc.begin() + (low * dist), ls.begin() + (loser_index - dist + 1), ls.begin() + (loser_index + 1));
}

static std::vector<int> produce_jacobsthal_sequence(std::vector<int> &ls, size_t dist)
{
	size_t prev = 1;
	size_t curr = 3;
	std::vector<int> js_sequence;
	js_sequence.push_back(curr);

	while(curr < (ls.size() / dist) + 1)
	{
		int next = curr + (2 * prev);
		js_sequence.push_back(next);
		prev = curr;
		curr = next;
	}
	return (js_sequence);
}

static void jacobsthal_insert(std::vector<int> &vect, size_t dist)
{
	std::vector<int> wc;
	std::vector<int> ls;
	std::vector<int> lefts;

	for (size_t i = 0; i < (vect.size() / dist); i++)
	{
		size_t start_i = (i * dist);
		size_t end_i = start_i + dist;
		if (i % 2 == 0 && i != 0)
			ls.insert(ls.end(), vect.begin() + start_i, vect.begin() + end_i);
		else
			wc.insert(wc.end(), vect.begin() + start_i, vect.begin() + end_i);
	}
	lefts.insert(lefts.end(), vect.begin() + ((vect.size() / dist) * dist), vect.end());
	std::vector<int> js_sequence = produce_jacobsthal_sequence(ls, dist);
	int last_iter = 1;
	int num_losers = ls.size() / dist;
	int count = 1;
	for (size_t i = 0; i < js_sequence.size(); i++)
	{
		int iter = js_sequence[i];
		if (iter > num_losers + 1)
			iter = num_losers + 1;
		for (int j = iter; j > last_iter; j--)
		{
			int max_chunk_index = j + count - 2;
			chunk_binary_search_insert(dist, wc, ls, ((j - 1) * dist) - 1, max_chunk_index);
			count++;
		}
		last_iter = iter;
	}
	vect.clear();
	vect.insert(vect.end(), wc.begin(), wc.end());
	vect.insert(vect.end(), lefts.begin(), lefts.end());
}


static void fj_sort_engine(std::vector<int> &vect, int layer)
{
	size_t	distance = static_cast<size_t>(1) << layer;
	size_t	i = (distance - 1);

	if ((distance * 2) > vect.size())
		return ;
	while((i + distance) < vect.size())
	{
		if (vect[i] > vect[i + distance])
			swap_range(vect, distance, (i - distance + 1), i + 1);
		i += (distance * 2);
	}
	fj_sort_engine(vect, layer + 1);
	jacobsthal_insert(vect, distance);
}

void PmergeMe::vect_sort()
{
	fj_sort_engine(vect, 0);
}

// ----------------------------------------------------------------- deque
static void swap_range_deq(std::deque<int> &deq_cont, size_t distance, size_t AC_start, size_t BC_start)
{
	std::swap_ranges(deq_cont.begin() + AC_start, deq_cont.begin() + AC_start + distance, deq_cont.begin() + BC_start);
}

static void chunk_binary_search_insert_deq(size_t dist, std::deque<int> &wc, std::deque<int> &ls, int loser_index, int max_chunk_index)
{
	int low = 0;
	int high = max_chunk_index;
	int mid;
	int target = ls[loser_index];

	while (low <= high)
	{
		mid = low + (high - low) / 2;
		int mid_key = wc[(mid * dist) + (dist - 1)];
		if (mid_key < target)
			low = mid + 1;
		else
			high = mid - 1;
	}
	wc.insert(wc.begin() + (low * dist), ls.begin() + (loser_index - dist + 1), ls.begin() + (loser_index + 1));
}

static std::vector<int> produce_jacobsthal_sequence_deq(std::deque<int> &ls, size_t dist)
{
	size_t prev = 1;
	size_t curr = 3;
	std::vector<int> js_sequence;
	js_sequence.push_back(curr);

	while(curr < (ls.size() / dist) + 1)
	{
		int next = curr + (2 * prev);
		js_sequence.push_back(next);
		prev = curr;
		curr = next;
	}
	return (js_sequence);
}

static void jacobsthal_insert_deq(std::deque<int> &deq_cont, size_t dist)
{
	std::deque<int> wc;
	std::deque<int> ls;
	std::deque<int> lefts;

	for (size_t i = 0; i < (deq_cont.size() / dist); i++)
	{
		size_t start_i = (i * dist);
		size_t end_i = start_i + dist;
		if (i % 2 == 0 && i != 0)
			ls.insert(ls.end(), deq_cont.begin() + start_i, deq_cont.begin() + end_i);
		else
			wc.insert(wc.end(), deq_cont.begin() + start_i, deq_cont.begin() + end_i);
	}
	lefts.insert(lefts.end(), deq_cont.begin() + ((deq_cont.size() / dist) * dist), deq_cont.end());

	std::vector<int> js_sequence = produce_jacobsthal_sequence_deq(ls, dist);

	int last_iter = 1;
	int num_losers = ls.size() / dist;
	int count = 1;

	for (size_t i = 0; i < js_sequence.size(); i++)
	{
		int iter = js_sequence[i];
		if (iter > num_losers + 1)
			iter = num_losers + 1;
		for (int j = iter; j > last_iter; j--)
		{
			int max_chunk_index = j + count - 2;
			chunk_binary_search_insert_deq(dist, wc, ls, ((j - 1) * dist) - 1, max_chunk_index);
			count++;
		}
		last_iter = iter;
	}
	deq_cont.clear();
	deq_cont.insert(deq_cont.end(), wc.begin(), wc.end());
	deq_cont.insert(deq_cont.end(), lefts.begin(), lefts.end());
}

static void fj_sort_engine_deq(std::deque<int> &deq_cont, int layer)
{
	size_t distance = static_cast<size_t>(1) << layer;
	size_t i = (distance - 1);

	if ((distance * 2) > deq_cont.size())
		return ;
	while((i + distance) < deq_cont.size())
	{
		if (deq_cont[i] > deq_cont[i + distance])
			swap_range_deq(deq_cont, distance, (i - distance + 1), i + 1);
		i += (distance * 2);
	}
	fj_sort_engine_deq(deq_cont, layer + 1);
	jacobsthal_insert_deq(deq_cont, distance);
}

void PmergeMe::deq_sort()
{
	fj_sort_engine_deq(deq, 0);
}

/* Each timer covers filling the container AND sorting it. */
void PmergeMe::run(int ac, char **av)
{
	parse_parameters(ac, av);

	std::clock_t start = std::clock();
	vect.assign(unsorted.begin(), unsorted.end());
	vect_sort();
	std::clock_t end = std::clock();
	vect_time = 1000000.0 * static_cast<double>(end - start) / CLOCKS_PER_SEC;

	start = std::clock();
	deq.assign(unsorted.begin(), unsorted.end());
	deq_sort();
	end = std::clock();
	deq_time = 1000000.0 * static_cast<double>(end - start) / CLOCKS_PER_SEC;

	if (!std::equal(deq.begin(), deq.end(), vect.begin()))
		throw std::runtime_error("Error");
}
