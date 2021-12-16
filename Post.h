#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <utility>
#include <iostream>
#include <queue>
#include <cassert>
#include <iostream>
#include <stack>
#include "Helpers.h"
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
		friend std::ostream& operator<<(std::ostream& os, Post const& node) {
			return os << node.asRoot();
		}

		class NodePrinter {
		public:
			enum class EPrintMode { Root, LevelOrder, PreOrder };
		private:
			Post const* _ptr;
			EPrintMode _mode;
		public:
			NodePrinter(Post const* node, EPrintMode mode = EPrintMode::Root)
				:_ptr(node), _mode(mode) {}
			std::ostream& printRoot(std::ostream& os) const;
			std::ostream& printLevelOrder(std::ostream& os) const;
			std::ostream& printPreOrder(std::ostream& os) const;
			friend std::ostream& operator<<(std::ostream& os, NodePrinter const& tree);
		};

		NodePrinter asRoot() const {
			return this;
		}

		NodePrinter asLevelOrder() const {
			return { this, NodePrinter::EPrintMode::LevelOrder };
		}

		NodePrinter asInOrder() const {
			return { this, NodePrinter::EPrintMode::PreOrder };
		}
	};
}
