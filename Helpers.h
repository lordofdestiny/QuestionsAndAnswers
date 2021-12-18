#pragma once
#include <iostream>
#include <string>
#include <queue>
#include <stack>

namespace cnt {
	template<typename T>
	inline std::stack<T>::value_type& takeFrom(std::stack<T>& stack) {
		auto value = stack.top();
		stack.pop();
		return value;
	}

	template<typename T>
	inline std::queue<T>::value_type& takeFrom(std::queue<T>& queue) {
		auto value = queue.front();
		queue.pop();
		return value;
	}

	inline std::ostream& separator(std::ostream& os) {
		return os << std::string(50, '-') << '\n';
	}
	inline auto safeIgnore(std::istream& is) {
		static bool firstPromt = true;
		if (!firstPromt) {
			is.ignore();
		}
		else {
			firstPromt = false;
		}
	}
	template<typename T>
	inline T promptValue(const char* text, std::ostream& os = std::cout, std::istream& is = std::cin) {
		os << text;
		T value;
		safeIgnore(is);
		is >> value;
		return value;
	}
	template<>
	inline std::string promptValue(const char* text, std::ostream& os, std::istream& is) {
		os << text;
		std::string str;
		safeIgnore(is);
		std::getline(is, str);
		return str;
	}
}