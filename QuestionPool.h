#pragma once
#include <vector>
#include "PostNode.h"

namespace qna {
	class QuestionPool
	{
	public:
		QuestionPool() : _questions() { };
		QuestionPool(QuestionPool const& rhs) {
			copyQuestions(rhs._questions);
		};
		QuestionPool(QuestionPool&& rhs) noexcept :
			_questions(std::move(rhs._questions)) {}
		QuestionPool& operator=(QuestionPool const& other) {
			if (this != &other) {
				_questions.clear();
				copyQuestions(other._questions);
			}
			return *this;
		}
		QuestionPool& operator=(QuestionPool&& other) noexcept {
			if (this != &other) {
				_questions = std::move(other._questions);
			}
			return *this;
		}
		void addQuestion(std::string);
		bool answer(PostNode*, std::string const&);
		PostNode* findQuestion(std::string);
		PostNode* findQuestion(unsigned long id);
		PostNode* findAnyById(unsigned long);
		PostNode* findHighestVotedResponse(std::string);
		PostNode* findHighestVotedResponse(unsigned long);
		bool upvote(unsigned long);
		void sortAnswers();
		void printQuestions();
		void printQuestion(std::string);
		void printQuestion(unsigned long);
		bool deleteQuestion(std::string);
		bool deleteQuestion(unsigned long);
		bool deleteResponse(unsigned long);
		static void printTree(PostNode*);

		std::size_t questionCount() const {
			return _questions.size();
		}
	private:
		void copyQuestions(std::vector<qna::PostNode*> const& from) {
			for (auto question : from) {
				_questions.push_back(new PostNode(*question));
			}
		}
		PostNode* findMostVotedInTree(PostNode*);
#ifdef REASSIGN
		void reassignParents();
#endif
		std::vector<qna::PostNode*> _questions;
	};
}
