#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <utility>
#include <iostream>
#include <queue>
#include "Answer.h"
#include "GlobalID.h"

namespace qna {
	class Answer;
	class Post
	{
	private:
		std::string _text;
		GlobalID _id;
		Post* _parent = nullptr;
		std::vector<qna::Answer> _answers;
	public:
		Post(std::string const& text, Post* parent = nullptr)
			: _text(text), _parent(parent) {}
		Post(Post const& rhs) = delete;
		Post(Post&& rhs) noexcept :
			_text(std::move(rhs._text)),
			_parent(rhs._parent),
			_answers(std::move(rhs._answers)) {}
		Post& operator=(Post const& other) = delete;
		Post& operator=(Post&&) noexcept;
		~Post();
	private:
		qna::Answer& findInParent() const;
	public:
		int votes() const {
			return findInParent().votes();
		}
		int upvote() {
			return findInParent().upvote();
		}
		bool isQuestion() const {
			return _parent == nullptr;
		}
		std::string& text() {
			return _text;
		}
		std::string const& text() const {
			return _text;
		}
		auto& answers() {
			return _answers;
		}
		auto const& answers() const {
			return _answers;
		}
		GlobalID::IDType id() const {
			return _id;
		}
		Post* parent() const {
			return _parent;
		}
		unsigned answerCount() const {
			return static_cast<unsigned>(_answers.size());
		}
		void printRoot(std::ostream& os = std::cout) const {
			os << *this;
		}
		void sort();
		bool answer(std::string const&) noexcept(false);
		friend std::ostream& operator<<(std::ostream& os, Post const& node);

		class Tree {
		private:
			Post const* _ptr;
		public:
			Tree(Post const* node) :_ptr(node) {}
			friend std::ostream& operator<<(std::ostream& os, Tree const& tree);
		};

		Tree asTree() const {
			return this;
		}
	};
}
