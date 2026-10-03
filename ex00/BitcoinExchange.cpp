#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &obj) : data(obj.data) {}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &obj)
{
	if (this != &obj)
		data = obj.data;
	return (*this);
}

BitcoinExchange::~BitcoinExchange() {}

static void stripCR(std::string &line)
{
	if (!line.empty() && line[line.size() - 1] == '\r')
		line.erase(line.size() - 1);
}

bool BitcoinExchange::isValidDate(const std::string &date)
{
	if (date.size() != 10 || date[4] != '-' || date[7] != '-')
		return (false);
	for (std::size_t i = 0; i < date.size(); i++)
	{
		if (i == 4 || i == 7)
			continue ;
		if (!std::isdigit(static_cast<unsigned char>(date[i])))
			return (false);
	}
	int year = std::atoi(date.substr(0, 4).c_str());
	int month = std::atoi(date.substr(5, 2).c_str());
	int day = std::atoi(date.substr(8, 2).c_str());
	int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if (month < 1 || month > 12)
		return (false);
	if (month == 2 && (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)))
		daysInMonth[1] = 29;
	return (day >= 1 && day <= daysInMonth[month - 1]);
}

bool BitcoinExchange::isValidValue(const std::string &value)
{
	std::size_t i = 0;
	int digits = 0;
	int dots = 0;

	if (value.empty())
		return (false);
	if (value[0] == '-' || value[0] == '+')
		i++;
	for (; i < value.size(); i++)
	{
		if (value[i] == '.')
			dots++;
		else if (std::isdigit(static_cast<unsigned char>(value[i])))
			digits++;
		else
			return (false);
	}
	return (digits > 0 && dots <= 1);
}

void BitcoinExchange::processLine(const std::string &line) const
{
	std::size_t pos = line.find(" | ");
	if (pos == std::string::npos)
		throw std::runtime_error("Error: bad input => " + line);

	std::string date = line.substr(0, pos);
	std::string value = line.substr(pos + 3);
	if (!isValidDate(date) || !isValidValue(value))
		throw std::runtime_error("Error: bad input => " + line);

	double val = std::strtod(value.c_str(), NULL);
	if (val < 0)
		throw std::runtime_error("Error: not a positive number.");
	if (val > 1000)
		throw std::runtime_error("Error: too large a number.");
	std::map<std::string, double>::const_iterator it = data.lower_bound(date);
	if (it == data.end() || it->first != date)
	{
		if (it == data.begin())
			throw std::runtime_error("Error: date not found in database => " + date);
		--it;
	}
	std::cout << date << " => " << val << " = " << val * it->second << std::endl;
}

void BitcoinExchange::parser(const std::string &filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
		throw std::runtime_error("Error: could not open file.");

	std::cout << std::setprecision(12);
	std::string line;
	bool firstLine = true;
	while (std::getline(file, line))
	{
		stripCR(line);
		if (firstLine)
		{
			firstLine = false;
			if (line == "date | value")
				continue ;
		}
		try
		{
			processLine(line);
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
		stripCR(line);
		std::size_t pos = line.find(',');
		if (pos == std::string::npos)
			continue ;
		std::string date = line.substr(0, pos);
		std::string rate = line.substr(pos + 1);
		if (!isValidDate(date) || !isValidValue(rate))
			continue ;
		data[date] = std::strtod(rate.c_str(), NULL);
	}
	if (data.empty())
		throw std::runtime_error("Error: database is empty or invalid.");
}
