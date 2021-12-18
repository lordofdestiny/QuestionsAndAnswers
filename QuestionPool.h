#pragma once
#include <vector>
#include <functional>
#include "Post.h"
#include "SearchFunction.h"

namespace qna {
	using cnt::takeFrom;

	template<typename T>
	concept QueryType =
		std::is_same_v<T, GlobalID::IDType>
		|| std::is_same_v<T, std::string>
		|| std::is_same_v<T, const char*>;

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
		bool addAnswer(T query, std::string text) {
			Post* node = findReal(std::forward<T>(query), ESearchType::Any);
			return node != nullptr ? node->answer(text) : false;
		}

		template<QueryType T>
		Post* findQuestion(T query) {
			return findReal(query, ESearchType::Question);
		}

		template<QueryType T>
		Post* findHighestVotedResponse(T query) {
			Post* node = findQuestion(query);
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
			auto maxNode = std::max_element(
				_questions.begin(), _questions.end(),
				[](Post* first, Post* second) {
					return first->totalResponseCount() < second->totalResponseCount();
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
		bool upvote(T query) {
			Post* node = findReal(query, ESearchType::Response);
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
		void printQuestion(T key, PrintMode mode = PrintMode::LevelOrder, std::ostream& os = std::cout) {
			Post* node = findQuestion(key);
			if (node == nullptr) {
				os << "Question not found!\n";
				return;
			}
			os << Post::PostPrinter(node, mode) << "\n";
		}

		template<QueryType T>
		bool deleteQuestion(T query) {
			auto condition = SearchFunction<T>{}(query);
			auto toDelete = std::find_if(_questions.begin(), _questions.end(), condition);
			if (toDelete == _questions.end()) return false;
			_questions.erase(toDelete);
			return true;
		}

		template<typename T>
		bool deleteResponse(T query) {
			Post* node = findReal(query, ESearchType::Response);
			auto condition = SearchFunction<T>{}(query);
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
		Post* findReal(T query, ESearchType stype) {
			using enum ESearchType;
			bool findQuestion = stype == Question;
			bool findResponse = stype == Response;
			bool findAny = stype == Any;
			auto userCondition = SearchFunction<T>{}(query);
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
