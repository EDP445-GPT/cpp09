#pragma once

#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <exception>
#include <cstdlib>

class BitcoinExchange {
	public :
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange &obj);
		BitcoinExchange &operator=(const BitcoinExchange &obj);
		~BitcoinExchange();
		void parser(char *filename);
		void loadDatabase(const std::string &filename);
	private:
		std::map<std::string, double> data;
};