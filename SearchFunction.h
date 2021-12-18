#pragma once
#include <string>
#include <functional>
#include "Post.h"

namespace qna {
	template<typename>
	struct SearchFunction;

	template<>
	struct SearchFunction<GlobalID::IDType> {
		auto operator()(GlobalID::IDType id) {
			return [id](Post* node) {
				return node->id() == id;
			};
		}
	};

	template<>
	struct SearchFunction<std::string> {
		auto operator()(std::string text) {
			return [text](Post* node) {
				return node->text() == text;
			};
		}
	};
	
	template<>
	struct SearchFunction<const char*> {
		auto operator()(std::string text) {
			return [text](Post* node) {
				return node->text() == text;
			};
		}
	};
}