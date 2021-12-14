#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <iterator>
#include "QuestionPool.h"
#include "Helpers.h"

using namespace qna;

QuestionPool::~QuestionPool() {
	for (auto question : _questions) {
		delete question;
	}
	_questions.clear();
}

void QuestionPool::addQuestion(std::string text) {
	_questions.push_back(new Post(text));
}

Post* QuestionPool::findQuestionReal(SearchLambda condition) {
	auto answer = std::find_if(_questions.begin(), _questions.end(), condition);
	return answer == _questions.end() ? nullptr : *answer;
}
Answer QuestionPool::findQuestion(GlobalID::IDType id) {
	return findQuestionReal(
		[&id](Post* node) {
			return node->id() == id;
		});
}
Answer QuestionPool::findQuestion(std::string text) {
	return findQuestionReal(
		[&text](Post* node)->bool {
			return node->text() == text;
		});
}

Post* QuestionPool::findResponseReal(SearchLambda condition) {
	std::queue<Post*> nodes;
	for (auto node : this->_questions) {
		nodes.push(node);
	}
	while (!nodes.empty()) {
		Post* node = nodes.front();
		nodes.pop();
		if (!node->isQuestion() && condition(node)) {
			return node;
		}
		for (auto reply : node->answers()) {
			nodes.push(reply);
		}
	}
	return nullptr;
}
Post* QuestionPool::findResponse(GlobalID::IDType id) {
	return findResponseReal(
		[&id](Post* node) -> bool {
			return node->id() == id;
		});
}
Post* QuestionPool::findResponse(std::string text) {
	return findResponseReal(
		[&text](Post* node) -> bool {
			return node->text() == text;
		});
}


Post* QuestionPool::findAnyReal(SearchLambda condition) {
	std::queue<Post*> nodes;
	for (auto node : this->_questions) {
		nodes.push(node);
	}
	while (!nodes.empty()) {
		Post* node = nodes.front();
		nodes.pop();
		if (condition(node)) {
			return node;
		}
		for (auto reply : node->answers()) {
			nodes.push(reply);
		}
	}
	return nullptr;
}
Post* QuestionPool::findAny(GlobalID::IDType id) {
	return findAnyReal(
		[&id](Post* node) -> bool {
			return node->id() == id;
		});
}
Post* QuestionPool::findAny(std::string text) {
	return findAnyReal(
		[&text](Post* node) -> bool {
			return node->text() == text;
		});
}

bool QuestionPool::addAnswerReal(Post* node, std::string& text) {
	if (node == nullptr) return false;
	node->answer(text);
	return true;
}

bool QuestionPool::addAnswer(GlobalID::IDType id, std::string answerText) {
	Post* node = findAny(id);
	return addAnswerReal(node, answerText);
}
bool QuestionPool::addAnswer(std::string text, std::string answerText) {
	Post* node = findAny(text);
	return addAnswerReal(node, answerText);
}

Post* QuestionPool::findHighestVotedInTree(Post* node) {
	if (node == nullptr) return nullptr;
	Post* maxNode = nullptr;
	std::queue<Post*> nodes;
	nodes.push(node);
	while (!nodes.empty()) {
		Post* node = nodes.front();
		nodes.pop();
		if (node == nullptr) continue;
		for (auto node : node->answers()) {
			nodes.push(node);
		}
		if (node->parent() == nullptr) continue;
		if (maxNode == nullptr || node->votes() > maxNode->votes()) {
			maxNode = node;
		}
	}
	return maxNode;
}
Answer QuestionPool::findHighestVotedResponse(GlobalID::IDType id) {
	Post* question = findQuestion(id);
	return findHighestVotedInTree(question);
}
Answer QuestionPool::findHighestVotedResponse(std::string text) {
	Post* question = findQuestion(text);
	return findHighestVotedInTree(question);
}

bool QuestionPool::upvoteReal(Post* node) {
	if (node == nullptr || node->parent() == nullptr) return false;
	node->upvote();
	return true;
}
bool QuestionPool::upvote(GlobalID::IDType id) {
	Post* node = findResponse(id);
	return upvoteReal(node);
}
bool QuestionPool::upvote(std::string text) {
	Post* node = findResponse(text);
	return upvoteReal(node);
}

void QuestionPool::sortAll() {
	for (auto& node : _questions) {
		node->sort();
	}
}

void QuestionPool::printQuestions(std::ostream& os) {
	for (auto& node : _questions) {
		os << *node << '\n';
	};
}

void QuestionPool::printQuestion(GlobalID::IDType id, EPrintMode mode, std::ostream& os) {
	printQuestionReal(id, mode, os);
}
void QuestionPool::printQuestion(std::string text, EPrintMode mode, std::ostream& os) {
	printQuestionReal(text, mode, os);
}

bool QuestionPool::deleteQuestionReal(SearchLambda condition) {
	auto toDelete = std::find_if(_questions.begin(), _questions.end(), condition);
	if (toDelete == _questions.end()) return false;
	_questions.erase(toDelete);
	return true;
}
bool QuestionPool::deleteQuestion(GlobalID::IDType id) {
	return deleteQuestionReal(
		[&id](Post* node) {
			return node->id() == id;
		});
}
bool QuestionPool::deleteQuestion(std::string text) {
	return deleteQuestionReal(
		[&text](Post* node) {
			return node->text() == text;
		});
}

bool QuestionPool::deleteResponse(GlobalID::IDType id) {
	return deleteResponseReal(
		id, [&id](Post* node) {
			return node->id() == id;
		});
}
bool QuestionPool::deleteResponse(std::string text) {
	return deleteResponseReal(
		text, [&text](Post* node) {
			return node->text() == text;
		});
}