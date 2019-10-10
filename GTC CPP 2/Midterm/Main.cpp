﻿#include <iostream>
#include "Date.hpp"

int main() {
	Date later(4, 31, 2000);
	Date earlier(6, 22, 1950);
	std::cout << later << std::endl;
	std::cout << ++later << " Prefix ++ " << std::endl;
	std::cout << --later << " Prefix -- " << std::endl;
	Date temp = later++;
	std::cout << temp;
	std::cout << " Postfix ++ " << later << std::endl;
	temp = later--;
	std::cout << temp;
	std::cout << " Postfix -- " << later << std::endl;
	std::cout << later << " - " << earlier << " = " << (later - earlier) << " days" << std::endl;
	std::cout << later.dayMonthFormat() << " dayMonthFormat" << std::endl;
	std::cout << later.monthDayFormat() << " monthDayFormat" << std::endl;
	std::cout << later.numberedFormat() << " numberedFormat" << std::endl;
	system("cmd /k");
}
