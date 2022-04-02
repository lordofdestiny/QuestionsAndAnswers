#pragma once
#include <iostream>
#include <string>
#include <queue>
#include <stack>

namespace cnt {
	template<typename T>
	inline typename std::stack<T>::value_type takeFrom(std::stack<T>& stack) {
		auto value = std::move(stack.top());
		stack.pop();
		return value;
	}

	template<typename T>
	inline typename std::queue<T>::value_type takeFrom(std::queue<T>& queue) {
		auto value = std::move(queue.front());
		queue.pop();
		return value;
	}

	inline std::ostream& separator(std::ostream& os) {
		return os << std::string(50, '-') << '\n';
	}

	template<typename T>
	inline T promptValue(const char* text, std::ostream& os = std::cout, std::istream& is = std::cin) {
		os << text;
		T value;
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
