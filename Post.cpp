#include "Post.h"

using namespace qna;

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
	std::sort(_answers.begin(), _answers.end(), std::greater<Answer>());
	for (auto answer : _answers) {
		answer->sort();
	}
}

bool Post::answer(std::string const& text) noexcept(false) {
	if (_answers.size() >= 10) return false;
	_answers.emplace_back(new Post(text, this));
	return true;
}

std::ostream& qna::operator<<(std::ostream& os, Post const& node) {
	std::string parent = node.parent() == nullptr
		? "R" : "P=" + std::to_string(node.parent()->id());
	os << "(" << parent << ", ID=" << node.id() << "): [\"";
	os << "Text: " << node.text() << "\", Answers: " << node.answerCount();
	if (node.parent() != nullptr) {
		os << ", Votes: " << node.votes();
	}
	return os << "]";
}

std::ostream& qna::operator<<(std::ostream& os, Post::LevelOdrer const& tree) {
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
		remainingInLevel = std::exchange(addedToLevel, 0);
		level++;
	}
	return os;
}

std::ostream& qna::operator<<(std::ostream& os, Post::PreOrder const& tree) {
	if (tree._ptr == nullptr) return os << "nulltpr";
	std::stack <std::pair<Post const*, unsigned>> nodes;
	nodes.emplace(tree._ptr, 0);
	while (!nodes.empty()) {
		auto [node, level] = nodes.top();
		nodes.pop();
		os << std::string(level, '\t') << *node << "\n";
		for (auto it = node->answers().rbegin(); it != node->answers().rend(); it++) {
			nodes.emplace(*it, level + 1);
		}
	}
	return os;
}
