#pragma once
#include <string>
#include <functional>
#include "Post.h"

namespace qna {
	template<typename T>
	concept QueryType =
		std::is_same_v<T, GlobalID::IDType>
		|| std::is_same_v<T, std::string>;
	
	template<QueryType T>
	struct SearchFunction {
		SearchFunction(T const& text) : _query(text) {}
		auto operator()(Post const* node) {
			return node->text() == _query;
		}
	private:
		std::string const& _query;
	};

	template<>
	struct SearchFunction<GlobalID::IDType> {
		SearchFunction(GlobalID::IDType id) :_query(id) {}
		auto operator()(Post const* node) {
			return node->id() == _query;
		}
	private:
		GlobalID::IDType _query;
	};
}