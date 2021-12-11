#pragma once
#include <iostream>
#include "QuestionPool.h"

namespace cnt {
	using namespace qna;
	void printMenu();
	int activateChoice(int, QuestionPool*&);
	inline void createPool(QuestionPool*& qpool) {
		if (qpool != nullptr) {
			delete qpool;
		}
		qpool = new qna::QuestionPool();
	}
	void addQuestion(QuestionPool*&);
	void answerQuestionOrResponse(QuestionPool*&);
	void searchQuestions(QuestionPool*&);
	inline void printQuestions(QuestionPool*& qpool) {
		if (qpool->questionCount() > 0) {
			std::cout << "All questions: \n";
			qpool->printQuestions();
		}
		else {
			std::cout << "There are no questions in the pool!\n";
		}
	}
	void findHighestRatedResponse(QuestionPool*&);
	void upvoteResponse(QuestionPool*&);
	void printQuestion(QuestionPool*&);
	void deleteQuestion(QuestionPool*&);
	void deleteResponse(QuestionPool*&);
}