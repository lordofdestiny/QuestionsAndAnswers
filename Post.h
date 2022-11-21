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
		unsigned childrenCount() const {
			return static_cast<unsigned>(_answers.size());
		}

		unsigned totalResponseCount() const {
			std::queue<Post const*> nodes;
			nodes.push(this);
			unsigned count = this->childrenCount();
			while (!nodes.empty()) {
				auto curr = nodes.front();
				nodes.pop();
				for (Post* node : curr->answers()) {
					nodes.push(node);
					count += node->childrenCount();
				}
			}
			return count;
		}

		void sort();
		bool answer(std::string const&) noexcept(false);
		friend std::ostream& operator<<(std::ostream& os, Post const& node) {
			return os << node.asRoot();
		}

		class PostPrinter {
		public:
			enum class EPrintMode { Root, PreOrder, InOrder, PostOrder, LevelOrder };
		private:
			Post const* _ptr;
			EPrintMode _mode;
		public:
			PostPrinter(Post const* node, EPrintMode mode = EPrintMode::Root)
				:_ptr(node), _mode(mode) {}
			std::ostream& printRoot(std::ostream& os) const;
			std::ostream& printLevelOrder(std::ostream& os) const;
			std::ostream& printPreOrder(std::ostream& os) const;
			std::ostream& printInOrder(std::ostream& os) const;
			std::ostream& printPostOrder(std::ostream& os) const;
			friend std::ostream& operator<<(std::ostream& os, PostPrinter const& tree);
		};

		using enum PostPrinter::EPrintMode;

		PostPrinter asRoot() const {
			return this;
		}
		PostPrinter asLevelOrder() const {
			return { this, LevelOrder };
		}
		PostPrinter asPreOrder() const {
			return { this, PreOrder };
		}
		PostPrinter asInOrder() const {
			return { this, InOrder };
		}
		PostPrinter asPostOrder() const {
			return { this, PostOrder };
		}
	private:
		static std::size_t treeHeight(Post const*);
	};
}
