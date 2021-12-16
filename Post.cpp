#include "Post.h"

namespace qna {

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
			Post* curr = nodes.front();
			nodes.pop();
			if (curr == nullptr) continue;
			std::sort(curr->_answers.begin(), curr->_answers.end(), std::greater());
			for (auto answer : curr->_answers) {
				nodes.push(answer);
			}
		}
	}

	bool Post::answer(std::string const& text) noexcept(false) {
		if (_answers.size() >= 10) return false;
		_answers.emplace_back(new Post(text, this));
		return true;
	}

	std::ostream& Post::NodePrinter::printRoot(std::ostream& os) const{
		std::string parent = _ptr->parent() == nullptr
			? "R" : "P=" + std::to_string(_ptr->parent()->id());
		os << "(" << parent << ", ID=" << _ptr->id() << "): [\"";
		os << "Text: " << _ptr->text() << "\", Answers: " << _ptr->answerCount();
		if (_ptr->parent() != nullptr) {
			os << ", Votes: " << _ptr->votes();
		}
		return os << "]";
	}
	std::ostream& Post::NodePrinter::printLevelOrder(std::ostream& os) const{
		if (_ptr == nullptr) return os << "nulltpr";
		std::queue <Post const*> nodes;
		nodes.push(_ptr);
		unsigned level = 0;
		unsigned addedToLevel = 0;
		unsigned remainingInLevel = 1;
		while (!nodes.empty()) {
			while (remainingInLevel > 0) {
				auto node = nodes.front();
				nodes.pop();
				--remainingInLevel;
				os << std::string(level, '\t') << *node << "\n";
				for (auto replies : node->answers()) {
					nodes.push(replies);
				}
				addedToLevel += node->answerCount();
			}
			remainingInLevel = std::exchange(addedToLevel, 0);
			level++;
		}
		return os;
	}
	std::ostream& Post::NodePrinter::printPreOrder(std::ostream& os) const{
		if (_ptr == nullptr) return os << "nulltpr";
		std::stack <std::pair<Post const*, unsigned>> nodes;
		nodes.emplace(_ptr, 0/*iniital level*/);
		while (!nodes.empty()) {
			auto [curr, level] = nodes.top();
			nodes.pop();
			os << std::string(level, '\t') << *curr << "\n";
			auto& cAnswers = curr->_answers;
			for (auto iter = cAnswers.rbegin(); iter != cAnswers.rend(); iter++) {
				nodes.emplace(*iter, level + 1);
			}
		}
		return os;
	}
	std::ostream& qna::operator<<(std::ostream& os, Post::NodePrinter const& tree) {
		if (tree._mode == Post::NodePrinter::EPrintMode::Root) {
			return tree.printRoot(os);
		}
		if (tree._mode == Post::NodePrinter::EPrintMode::LevelOrder) {
			return tree.printLevelOrder(os);
		}
		if (tree._mode == Post::NodePrinter::EPrintMode::PreOrder) {
			return tree.printPreOrder(os);
		}
		return os;
	}
}
