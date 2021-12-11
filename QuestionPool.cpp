#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <iterator>
#include "QuestionPool.h"
#include "Helpers.h"

using namespace qna;

void QuestionPool::addQuestion(std::string text) {
	_questions.push_back(new PostNode(text, EPostType::Question));
}

PostNode* QuestionPool::findAnyById(unsigned long id) {
	std::queue<PostNode*> nodes;
	for (auto node : this->_questions) {
		nodes.push(node);
	}
	while (!nodes.empty()) {
		PostNode* node = nodes.front();
		nodes.pop();
		if (node->post().id() == id) {
			return node;
		}
		for (auto reply : node->answers()) {
			nodes.push(reply);
		}
	}
	return nullptr;
}

bool QuestionPool::answer(PostNode* node, std::string const& text) {
	if (node == nullptr) return false;
	auto newType = node->post().type() == EPostType::Question
		? EPostType::Answer : EPostType::Comment;
	node->answerPost(text, newType);
#ifdef REASSIGN
	reassignParents();
#endif
	return true;
}

#ifdef REASSIGN
void QuestionPool::reassignParents() {
	std::queue<PostNode*> nodes;
	for (auto question : _questions) {
		question->_parent = nullptr;
		nodes.push(question);
	}
	while (!nodes.empty()) {
		PostNode* node = nodes.front();
		nodes.pop();
		for (auto child : node->answers()) {
			child->_parent = node;
			nodes.push(child);
		}
	}
}
#endif

PostNode* QuestionPool::findQuestion(std::string text) {
	auto answer = std::find_if(_questions.begin(), _questions.end(), 
		[&](PostNode* node) {
			return node->post().text() == text;
		});
	return answer == _questions.end() ? nullptr : *answer;
}

PostNode* QuestionPool::findQuestion(unsigned long id) {
	auto answer = std::find_if(_questions.begin(), _questions.end(),
		[&](PostNode* node) {
			return node->post().id() == id;
		});
	return answer == _questions.end() ? nullptr : *answer;
}

PostNode* QuestionPool::findMostVotedInTree(PostNode* node) {
	if (node == nullptr) return nullptr;
	PostNode* maxNode = nullptr;
	std::queue<PostNode*> nodes;
	nodes.push(node);
	while (!nodes.empty()) {
		PostNode* node = nodes.front();
		nodes.pop();
		if (node == nullptr) continue;
		if (node->post().type() != EPostType::Question) {
			if (maxNode == nullptr || node->post().votes() > maxNode->post().votes()) {
				maxNode = node;
			}
		}
		for (auto node : node->answers()) {
			nodes.push(node);
		}
	}
	return maxNode;
}

PostNode* QuestionPool::findHighestVotedResponse(std::string text) {
	PostNode* question = findQuestion(text);
	return findMostVotedInTree(question);
}

PostNode* QuestionPool::findHighestVotedResponse(unsigned long id) {
	PostNode* question = findQuestion(id);
	return findMostVotedInTree(question);
}

bool QuestionPool::upvote(unsigned long id) {
	PostNode* node = findAnyById(id);
	if (node->post().type() == EPostType::Question) return false;
	node->post().upvote();
	return true;
}

void QuestionPool::sortAnswers() {
	for (auto& node : _questions) {
		node->sortResponses();
	}
}

void QuestionPool::printQuestions() {
	for (auto& node : _questions) {
		std::cout << node << '\n';
	};
}

void QuestionPool::printTree(PostNode* node) {
	if (node == nullptr) return;
	std::queue <PostNode*> nodes;
	nodes.push(node);
	unsigned level = 0;
	unsigned addedToLevel = 0, remainingInLevel = 1;
	while (!nodes.empty()) {
		while (remainingInLevel > 0) {
			PostNode* node = nodes.front();
			nodes.pop();
			remainingInLevel--;
			std::cout << *node << "\t";
			for (auto replies : node->answers()) {
				nodes.push(replies);
			}
			addedToLevel += node->answerCount();
		}
		std::cout << "\n";
		remainingInLevel = std::exchange(addedToLevel, 0);
		level++;
	}
	std::cout << '\n';
}

void QuestionPool::printQuestion(std::string text) {
	PostNode* node = findQuestion(text);
	printTree(node);
}

void QuestionPool::printQuestion(unsigned long id) {
	PostNode* node = findAnyById(id);
	printTree(node);
}

bool QuestionPool::deleteQuestion(std::string text) {
	auto toDelete = std::find_if(_questions.begin(), _questions.end(),
		[&](PostNode* node) {
			return node->post().text() == text;
		});
	if (toDelete == _questions.end()) return false;
	_questions.erase(toDelete);
#ifdef REASSIGN
	reassignParents();
#endif
	return true;
}

bool QuestionPool::deleteQuestion(unsigned long id) {
	auto toDelete = std::find_if(_questions.begin(), _questions.end(),
		[&](PostNode* node) {
			return node->post().id() == id;
		});
	if (toDelete == _questions.end()) return false;
	_questions.erase(toDelete);
#ifdef REASSIGN
	reassignParents();
#endif
	return true;
}

bool QuestionPool::deleteResponse(unsigned long id) {
	PostNode* node = findAnyById(id);
	if (node == nullptr) return false;
	if (node->parent() == nullptr) return false;
	if (node->post().type() == EPostType::Question) return false;
	auto toDelete = std::find_if(
		node->parent()->answers().begin(),
		node->parent()->answers().end(),
		[&](PostNode* node) {
			return node->post().id() == id;
		});
	if (toDelete == node->parent()->answers().end()) return false;
	node->parent()->answers().erase(toDelete);
#ifdef REASSIGN
	reassignParents();
#endif
	return true;
};
