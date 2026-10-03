#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <iostream>
# include <fstream>
# include <iomanip>
# include <map>
# include <string>
# include <stdexcept>
# include <cstdlib>
# include <cctype>

class BitcoinExchange
{
	public:
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange &obj);
		BitcoinExchange &operator=(const BitcoinExchange &obj);
		~BitcoinExchange();

		void loadDatabase(const std::string &filename);
		void parser(const std::string &filename);

	private:
		std::map<std::string, double> data;

		static bool isValidDate(const std::string &date);
		static bool isValidValue(const std::string &value);
		void processLine(const std::string &line) const;
};

#endif
