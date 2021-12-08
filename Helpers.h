#include <iostream>
#include <string>

namespace cnt{
	inline std::ostream& separator(std::ostream& os) {
		return os << std::string(50, '-') << '\n';
	}
}