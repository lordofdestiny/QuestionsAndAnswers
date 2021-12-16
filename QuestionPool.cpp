#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <iterator>
#include "QuestionPool.h"
#include "Helpers.h"

namespace qna {
	QuestionPool::~QuestionPool() {
		for (auto question : _questions) {
			delete question;
		}
		_questions.clear();
	}

	void QuestionPool::addQuestion(std::string text) {
		_questions.push_back(new Post(text));
	}

	Post* QuestionPool::findReal(SearchLambda userCondition, ESearchType stype) {
		bool findQuestion = stype == ESearchType::Question;
		bool findResponse = stype == ESearchType::Response;
		bool findAny = stype == ESearchType::Any;
		SearchLambda condition = [&](Post* node)->bool {
			return (
				(findQuestion && node->isQuestion() && userCondition(node))
				|| (findResponse && !node->isQuestion() && userCondition(node))
				|| (findAny && userCondition(node)));
		};
		std::queue<Post*> nodes;
		for (auto node : _questions) {
			nodes.push(node);
		}
		Post* result = nullptr;
		while (!nodes.empty()) {
			Post* curr = nodes.front();
			nodes.pop();
			if (condition(curr)) {
				result = curr;
				break;
			}
			for (auto answer : curr->answers()) {
				nodes.push(answer);
			}
		}
		return result;
	}

	Answer QuestionPool::findQuestion(GlobalID::IDType id) {
		return findReal(makeIdCmp(id), ESearchType::Question);
	}
	Answer QuestionPool::findQuestion(std::string text) {
		return findReal(makeTextCmp(text), ESearchType::Question);
	}

	Post* QuestionPool::findResponse(GlobalID::IDType id) {
		return findReal(makeIdCmp(id), ESearchType::Response);
	}
	Post* QuestionPool::findResponse(std::string text) {
		return findReal(makeTextCmp(text), ESearchType::Response);
	}

	Post* QuestionPool::findAny(GlobalID::IDType id) {
		return findReal(makeIdCmp(id), ESearchType::Any);
	}
	Post* QuestionPool::findAny(std::string text) {
		return findReal(makeTextCmp(text), ESearchType::Any);
	}

	bool QuestionPool::addAnswerReal(Post* node, std::string& text) {
		if (node == nullptr) return false;
		node->answer(text);
		return true;
	}

	bool QuestionPool::addAnswer(GlobalID::IDType id, std::string answerText) {
		return addAnswerReal(findAny(id), answerText);
	}
	bool QuestionPool::addAnswer(std::string text, std::string answerText) {
		return addAnswerReal(findAny(text), answerText);
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

	bool QuestionPool::deleteQuestionReal(SearchLambda condition) {
		auto toDelete = std::find_if(_questions.begin(), _questions.end(), condition);
		if (toDelete == _questions.end()) return false;
		_questions.erase(toDelete);
		return true;
	}
	bool QuestionPool::deleteQuestion(GlobalID::IDType id) {
		return deleteQuestionReal(makeIdCmp(id));
	}
	bool QuestionPool::deleteQuestion(std::string text) {
		return deleteQuestionReal(makeTextCmp(text));
	}
	bool QuestionPool::deleteResponse(GlobalID::IDType id) {
		return deleteResponseReal(id, makeIdCmp(id));
	}
	bool QuestionPool::deleteResponse(std::string text) {
		return deleteResponseReal(text, makeTextCmp(text));
	}
}
