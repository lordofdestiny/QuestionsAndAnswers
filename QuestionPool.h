#pragma once
#include <vector>
#include <unordered_map>
#include <functional>
#include "Post.h"
#include "SearchFunction.h"

namespace qna {
	using cnt::takeFrom;

	class QuestionPool
	{
		using PrintMode = Post::PostPrinter::EPrintMode;
		enum class ESearchType { Any, Question, Response };
	private:
		std::vector<Post*> _questions;
	public:
		QuestionPool() : _questions() { };
		QuestionPool(QuestionPool const&) = delete;
		QuestionPool(QuestionPool&& rhs) noexcept :
			_questions(std::move(rhs._questions)) {}
		QuestionPool& operator=(QuestionPool const&) = delete;
		QuestionPool& operator=(QuestionPool&& other) noexcept {
			if (this != &other) {
				_questions = std::move(other._questions);
			}
			return *this;
		}

		~QuestionPool() {
			for (auto question : _questions) {
				delete question;
			}
			_questions.clear();
		}

		void addQuestion(std::string text) {
			_questions.push_back(new Post(text));
		}

		template<QueryType T>
		bool addAnswer(T const& _query, std::string text) {
			Post* node = find(std::forward<T const&>(_query), ESearchType::Any);
			return node != nullptr ? node->answer(text) : false;
		}

		template<QueryType T>
		Post* findQuestion(T const& _query) {
			return find(_query, ESearchType::Question);
		}

		template<QueryType T>
		Post* findHighestVotedResponse(T const& _query) {
			Post* node = findQuestion(_query);
			if (node == nullptr) return nullptr;
			Post* maxNode = nullptr;
			std::queue<Post*> nodes;
			nodes.push(node);
			while (!nodes.empty()) {
				Post* node = takeFrom(nodes);
				if (node == nullptr) continue;
				for (auto node : node->answers()) {
					nodes.push(node);
				}
				if (node->isQuestion()) continue;
				if (maxNode == nullptr || node->votes() > maxNode->votes()) {
					maxNode = node;
				}
			}
			return maxNode;
		}

		Post* findMostAnsweredQuestion() {
			if (_questions.empty()) return nullptr;
			std::unordered_map<Post*, unsigned> maxVotes;
			std::transform(_questions.begin(), _questions.end(),
				std::inserter(maxVotes, maxVotes.end()),
				[this](Post* const& node) {
					return std::make_pair(node, node->totalResponseCount());
				});
			auto maxNode = std::max_element(
				_questions.begin(), _questions.end(),
				[&maxVotes](Post* first, Post* second) {
					return maxVotes[first] < maxVotes[second];
				});
			if (maxNode == _questions.end()) return nullptr;
			return *maxNode;
		}

		Post* findQuestionWithHighestVotedResponse() {
			if (_questions.empty()) return nullptr;
			auto maxNode = std::max_element(
				_questions.begin(), _questions.end(),
				[this](Post* first, Post* second) {
					auto highestVotedFirst = findHighestVotedResponse(first->id());
					auto highestVotedSecond = findHighestVotedResponse(second->id());
					return highestVotedFirst->votes() < highestVotedSecond->votes();
				});
			if (maxNode == _questions.end()) return nullptr;
			return *maxNode;
		}

		template<QueryType T>
		bool upvote(T const& _query) {
			Post* node = find(_query, ESearchType::Response);
			if (node == nullptr || node->isQuestion()) return false;
			node->upvote();
			return true;
		}

		void sortAll() {
			for (auto& node : _questions) {
				node->sort();
			}
		}

		void printQuestions(std::ostream& os = std::cout) {
			for (auto& node : _questions) {
				os << *node << '\n';
			};
		}

		template<typename T>
		void printQuestion(T const& query, PrintMode mode = PrintMode::LevelOrder, std::ostream& os = std::cout) {
			Post* node = findQuestion(query);
			if (node == nullptr) {
				os << "Question not found!\n";
				return;
			}
			os << Post::PostPrinter(node, mode) << "\n";
		}

		template<QueryType T>
		bool deleteQuestion(T const& query) {
			SearchFunction<T> condition(query);
			auto toDelete = std::find_if(_questions.begin(), _questions.end(), condition);
			if (toDelete == _questions.end()) return false;
			_questions.erase(toDelete);
			return true;
		}

		template<typename T>
		bool deleteResponse(T const& query) {
			Post* node = find(query, ESearchType::Response);
			SearchFunction<T> condition(query);
			if (node == nullptr || node->isQuestion()) return false;
			auto& pAnswers = node->parent()->answers();
			auto toDelete = std::find_if(pAnswers.begin(), pAnswers.end(), condition);
			if (toDelete == pAnswers.end()) return false;
			pAnswers.erase(toDelete);
			return true;
		}

		std::size_t questionCount() const {
			return _questions.size();
		}
	private:
		template<QueryType T>
		Post* find(T const& query, ESearchType stype) {
			using enum ESearchType;
			bool findQuestion = stype == Question;
			bool findResponse = stype == Response;
			bool findAny = stype == Any;
			SearchFunction<T> userCondition(query);
			auto condition = [&](Post* node) {
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
				Post* curr = takeFrom(nodes);
				if (condition(curr)) {
					result = curr;
					break;
				}
				if (findQuestion) continue;
				for (auto answer : curr->answers()) {
					nodes.push(answer);
				}
			}
			return result;
		}
	};
}
