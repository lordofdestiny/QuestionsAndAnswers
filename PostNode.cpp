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

void PostNode::sortResponses() {
	std::sort(_answers.begin(), _answers.end(),
		[](PostNode* one, PostNode* two) {
			return one->post().votes() > one->post().votes();
		});
	for (auto answer : _answers) {
		answer->sortResponses();
	}
}

bool PostNode::answerPost(std::string const& text, EPostType type = EPostType::Comment) {
	if (_answers.size() >= 10) return false;
	_answers.push_back(new PostNode(text, type, this));
	return true;
}

void PostNode::copyResponses(std::vector<qna::PostNode*> const& from) {
	for (auto answer : from) {
		_answers.push_back(new PostNode(*answer));
	}
}

void PostNode::freeResponses() {
	for (auto answer : _answers) {
		delete answer;
	}
	_answers.clear();
}