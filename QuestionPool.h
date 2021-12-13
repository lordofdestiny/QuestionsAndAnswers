#pragma once
#include <vector>
#include <functional>
#include "Post.h"

namespace qna {
	class QuestionPool
	{
	private:
		using SearchLambda = std::function<bool(Post*)>;
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
		~QuestionPool();
		void addQuestion(std::string);
		bool postAnswer(unsigned long, std::string);
		bool postAnswer(std::string, std::string);
		Answer findQuestion(std::string);
		Answer findQuestion(unsigned long);
		Answer findHighestVotedResponse(std::string);
		Answer findHighestVotedResponse(unsigned long);
		bool upvote(unsigned long);
		bool upvote(std::string);
		void sortAll();
		void printQuestions(std::ostream& os = std::cout);
		void printQuestion(std::string, std::ostream& os = std::cout);
		void printQuestion(unsigned long, std::ostream& os = std::cout);
		bool deleteQuestion(std::string);
		bool deleteQuestion(unsigned long);
		bool deleteResponse(std::string);
		bool deleteResponse(unsigned long);

		std::size_t questionCount() const {
			return _questions.size();
		}
	private:
		Post* findQuestionReal(SearchLambda);
		Post* findResponseReal(SearchLambda);
		Post* findResponse(std::string);
		Post* findResponse(unsigned long);
		Post* findAnyReal(SearchLambda);
		Post* findAny(std::string);
		Post* findAny(unsigned long);
		bool postAnswerReal(Post* question, std::string& text);	
		Post* findHighestVotedInTree(Post*);
		bool upvoteReal(Post*);
		bool deleteQuestionReal(SearchLambda);
		template<typename T>
		bool deleteResponseReal(T parameter, SearchLambda condition) {
			Post* node = findResponse(parameter);
			if (node == nullptr || node->parent() == nullptr) return false;
			auto toDelete = std::find_if(
				node->parent()->answers().begin(),
				node->parent()->answers().end(),
				condition);
			if (toDelete == node->parent()->answers().end()) return false;
			node->parent()->answers().erase(toDelete);
			return true;
		}
	};
}
