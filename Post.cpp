#include "Post.h"

namespace qna {
	using cnt::takeFrom;
	Post::~Post() {
		for (auto& answer : _answers) {
			delete static_cast<Post*>(answer);
		}
		_answers.clear();
	}

	qna::Answer& Post::findInParent() const {
		assert((_parent != nullptr));
		auto result = std::find(
			_parent->_answers.begin(),
			_parent->_answers.end(), this);
		return *result;
	}

	Post& Post::operator=(Post&& other) noexcept {
		if (this != &other) {
			_text = std::move(other._text);
			_parent = std::exchange(other._parent, nullptr);
			_answers = std::move(other._answers);
		}
		return *this;
	}

	void Post::sort() {
		std::queue<Post*> nodes;
		nodes.push(this);
		while (!nodes.empty()) {
			Post* curr = takeFrom(nodes);
			auto& cAnswers = curr->answers();
			std::sort(cAnswers.begin(), cAnswers.end(), std::greater<Answer>());
			for (auto answer : cAnswers) {
				nodes.push(answer);
			}
		}
	}

	bool Post::answer(std::string const& text) noexcept(false) {
		if (_answers.size() >= 10) return false;
		_answers.emplace_back(new Post(text, this));
		return true;
	}

	std::ostream& Post::PostPrinter::printRoot(std::ostream& os) const {
		if (_ptr == nullptr) return os << "nullptr";
		std::string parent = _ptr->isQuestion()
			? "R" : "P=" + std::to_string(_ptr->parent()->id());
		os << "(" << parent << ", ID=" << _ptr->id() << "): [\"";
		os << "Text: " << _ptr->text() << "\", Answers: " << _ptr->childrenCount();
		if (_ptr->parent() != nullptr) {
			os << ", Votes: " << _ptr->votes();
		}
		return os << "]";
	}
	std::ostream& Post::PostPrinter::printLevelOrder(std::ostream& os) const {
		if (_ptr == nullptr) return os << "nulltpr";
		std::queue <Post const*> nodes;
		nodes.push(_ptr);
		unsigned level = 0;
		unsigned addedToLevel = 0;
		unsigned remainingInLevel = 1;
		while (!nodes.empty()) {
			while (remainingInLevel > 0) {
				auto node = takeFrom(nodes);
				--remainingInLevel;
				os << std::string(2 * level, '\t') << *node << "\n";
				for (auto replies : node->answers()) {
					nodes.push(replies);
				}
				addedToLevel += node->childrenCount();
			}
			remainingInLevel = std::exchange(addedToLevel, 0);
			level++;
		}
		return os;
	}
	std::ostream& Post::PostPrinter::printPreOrder(std::ostream& os) const {
		if (_ptr == nullptr) return os << "nulltpr";
		std::stack <std::pair<Post const*, unsigned>> nodes;
		nodes.emplace(_ptr, 0/*iniital level*/);
		while (!nodes.empty()) {
			auto [curr, level] = takeFrom(nodes);
			os << std::string(2 * level, '\t') << *curr << "\n";
			auto& cAnswers = curr->answers();
			for (auto iter = cAnswers.rbegin(); iter != cAnswers.rend(); iter++) {
				nodes.emplace(*iter, level + 1);
			}
		}
		return os;
	}
	std::ostream& Post::PostPrinter::printInOrder(std::ostream& os) const {
		if (_ptr == nullptr) return os << "nullptr";
		// std::size_t height = Post::treeHeight(_ptr);
		std::stack<std::tuple<Post const*, std::size_t, bool>> nodes;
		nodes.emplace(_ptr, 0/*initlevel*/, false);
		while (!nodes.empty()) {
			auto [curr, currLvl, visited] = takeFrom(nodes);
			auto& cAnswers = curr->answers();
			if (!visited) {
				if (!cAnswers.empty()) {
					nodes.emplace(cAnswers.back(), currLvl + 1, false);
				}
				nodes.emplace(curr, currLvl, true);
				for (int i = curr->childrenCount() - 2; i >= 0; i--) {
					nodes.emplace(cAnswers[i], currLvl + 1, false);
				}
			}
			else {
				os << std::string(2 * currLvl, '\t') << *curr << '\n';
			}
		}
		return os;
	}
	std::ostream& Post::PostPrinter::printPostOrder(std::ostream& os) const {
		if (_ptr == nullptr) return os << "nullptr";
		// std::size_t height = Post::treeHeight(_ptr);
		std::stack<std::tuple<Post const*, std::size_t, bool>> nodes;
		nodes.emplace(_ptr, 0/*init level*/, false);
		while (!nodes.empty())
		{
			auto [curr, currLvl, visited] = takeFrom(nodes);
			auto& cAnswers = curr->answers();
			if (!visited) {
				nodes.emplace(curr, currLvl, true);
				for (auto iter = cAnswers.rbegin(); iter != cAnswers.rend(); iter++) {
					nodes.emplace(*iter, currLvl + 1, false);
				}
			}
			else {
				os << std::string(2 * currLvl, '\t') << *curr << '\n';
			}
		}
		return os;
	}
	std::ostream& operator<<(std::ostream& os, Post::PostPrinter const& tree) {
		switch (tree._mode) {
		case Post::PostPrinter::EPrintMode::Root:
			return tree.printRoot(os);
		case Post::PostPrinter::EPrintMode::LevelOrder:
			return tree.printLevelOrder(os);
		case Post::PostPrinter::EPrintMode::PreOrder:
			return tree.printPreOrder(os);
		case Post::PostPrinter::EPrintMode::InOrder:
			return tree.printInOrder(os);
		case Post::PostPrinter::EPrintMode::PostOrder:
			return tree.printPostOrder(os);
		default:
			return os;
		}
	}

	std::size_t Post::treeHeight(Post const* root) {
		std::stack<std::pair<Post const*, std::size_t>> nodes;
		nodes.emplace(root, 0/*init level*/);
		std::size_t maxLevel = 0;
        // std::size_t prevLvl = -1;
		while (!nodes.empty()) {
			auto [curr, currLvl] = takeFrom(nodes);
			if (curr->childrenCount() == 0) {
				if (currLvl > maxLevel) {
					maxLevel = currLvl;
				}
			}
			else {
				for (auto answer : curr->answers()) {
					nodes.emplace(answer, currLvl + 1);
				}
			}
		}
		return maxLevel;
	}
}
