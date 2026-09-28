#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &obj)
{
	data = obj.data;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &obj)
{
	if (this == &obj)
		return (*this);
	data = obj.data;
	return (*this);
}

BitcoinExchange::~BitcoinExchange() {}

int is_number(const std::string &str)
{
	for (std::size_t i = 0; i < str.length(); i++)
	{
		if (!std::isdigit(static_cast<unsigned char>(str[i])))
			return (false);
	}
	return (true);
}

bool parse_value(const std::string &str)
{
	int counter = 0;
	
	if (str.empty() || str == ".")
		return false;

	for (std::size_t i = 0; i < str.length(); i++)
	{
		if (i == 0 && (str[i] == '-' || str[i] == '+'))
			continue;

		if (str[i] == '.')
		{
			counter++;
			if (counter > 1)
				return false;
			continue;
		}

		if (!std::isdigit(static_cast<unsigned char>(str[i])))
			return false;
	}
	return true;
}

void check_date_value(std::string line, size_t pos)
{
	std::string first  = line.substr(0, (pos - 1));
	std::string second  = line.substr(pos + 2);
	int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if (first.size() != 10 || first[4] != '-' || first[7] != '-')
		throw std::runtime_error("data error2");
	std::size_t first_delimiter = first.find("-");
	std::size_t second_delimiter = first.find("-", first_delimiter + 1);
	std::string string_year = first.substr(0, first_delimiter);
	std::string string_month = first.substr(first_delimiter + 1, second_delimiter - first_delimiter - 1);
	std::string string_day = first.substr(second_delimiter + 1);
	if (!is_number(string_year) || !is_number(string_month) || !(is_number(string_day)))
		throw std::runtime_error("data error3");
	int year = std::atoi(string_year.c_str()), month = std::atoi(string_month.c_str()), day = std::atoi(string_day.c_str());
	if (!(month >= 1 && month <= 12))
		throw std::runtime_error("data error4");
	if (month == 2 && (year % 4 == 0 && ((year % 100 != 0) || (year % 400 == 0))))
		daysInMonth[month - 1] = 29;
	if (day <= 0 || day > daysInMonth[month - 1])
		throw std::runtime_error("data error5");
	if (second.size() == 0 || !parse_value(second))
		throw std::runtime_error("data error5");
}

void BitcoinExchange::parser(char *filename)
{
	std::string line;
	std::ifstream fptr(filename);
	if (!fptr.is_open())
		throw std::runtime_error("couldn't open file");
	std::string first_line;
	std::getline(fptr, first_line);
	if (first_line != "date | value")
		throw std::runtime_error("wrong first line");
	while(getline (fptr, line))
	{
		try 
		{
			std::size_t pos = line.find("|");
			if (pos == std::string::npos)
				throw std::runtime_error("Error: bad input => " + line);
			check_date_value(line, pos);
			std::string first = line.substr(0, (pos - 1));
			std::string second = line.substr(pos + 2);
			double val = std::atof(second.c_str());
			if (val < 0)
				throw std::runtime_error("Error: not a positive number.");
			if (val > 1000)
				throw std::runtime_error("Error: too large a number.");
			std::map<std::string, double>::iterator it = data.lower_bound(first);
			if (it == data.begin() && it->first != first)
			{
				throw std::runtime_error("Error: date not found in database => " + first);
			}
			if (it == data.end() || it->first != first)
			{
				--it;
			}
d			std::cout << first << " => " << val << " = " << result << std::endl;
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
}

void BitcoinExchange::loadDatabase(const std::string &filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
		throw std::runtime_error("Error: could not open database file.");
	std::string line;
	std::getline(file, line);
	while (std::getline(file, line))
	{
		std::size_t pos = line.find(",");
		if (pos == std::string::npos)
			continue;
		std::string date = line.substr(0, pos);
		std::string rate = line.substr(pos + 1);
		data[date] = std::atof(rate.c_str());
	}
}