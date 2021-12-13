#include <cassert>
#include <iostream>
#include "Post.h"
#include "Helpers.h"

using namespace qna;

Post::~Post() {
	for (auto& answer : _answers) {
		delete static_cast<Post*>(answer);
	}
	_answers.clear();
}

qna::Answer& Post::findInParent() const {
	assert(_parent == nullptr, "A root represents a question and it cannot be upvoted!");
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
	std::sort(_answers.begin(), _answers.end());
	for (auto answer : _answers) {
		answer->sort();
	}
}

bool Post::answer(std::string const& text) noexcept(false) { // Needs to be fixed
	if (_answers.size() >= 10) return false;
	_answers.emplace_back(new Post(text, this));
	return true;
}

std::ostream& qna::operator<<(std::ostream& os, Post const& node) {
	std::string parent = node.parent() == nullptr
		? "R" : std::to_string(node.parent()->id());
	os << "(" << parent << ", ID=" << node.id() << "): [\"";
	os << node.text() << "\", Ans{" << node.answerCount() << "}";
	if (node.parent() != nullptr) {
		os << ", Vote{" << node.votes() << "}";
	}
	return os << "]";
}

std::ostream& qna::operator<<(std::ostream& os, Post::Tree const& tree) {
	if (tree._ptr == nullptr) return os << "nulltpr";
	std::queue <Post const*> nodes;
	nodes.push(tree._ptr);
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
		os << "\n";
		remainingInLevel = std::exchange(addedToLevel, 0);
		level++;
	}
	return os;
}