#pragma once
#include <iostream>
#include <string>

namespace cnt {

	inline std::ostream& separator(std::ostream& os) {
		return os << std::string(50, '-') << '\n';
	}

	template<typename T>
	inline T promptValue(const char* text, std::ostream& os = std::cout, std::istream& is = std::cin) {
		os << text;
		T value{};
		is >> value;
		is.ignore();
		return value;
	}

	template<>
	inline std::string promptValue(const char* text, std::ostream& os, std::istream& is) {
		os << text;
		std::string str;
		std::getline(is, str);
		return str;
	}

}
