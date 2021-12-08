#include "PostNode.h"

using namespace qna;

PostNode& PostNode::operator=(PostNode&& other) noexcept {
	if (this != &other) {
		_post = std::move(other._post);
		_parent = other._parent;
		_answers = std::move(other._answers);
	}
	return *this;
}

void PostNode::sortAnswers() {
	std::sort(_answers.begin(), _answers.end(),
		[](PostNode const& one, PostNode const& two) {
			return one.post().votes() > one.post().votes();
		});
	for (auto& answer : _answers) {
		answer.sortAnswers();
	}
}

bool PostNode::answerPost(std::string const& text, EPostType type = EPostType::Comment) {
	if (_answers.size() >= 10) return false;
	_answers.emplace_back(text, type, this);
	return true;
}

void PostNode::copyAnswers(std::vector<qna::PostNode> const& from) {
	for (auto& answer : from) {
		_answers.emplace_back(answer);
	}
}