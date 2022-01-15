#pragma once
#include <iostream>
#include <utility>
#include "QuestionPool.h"

namespace cnt {
	using namespace qna;
	void printMenu();
	void textOrIdMenuHandler(std::function<void(int)> handle);
	int activateChoice(int, QuestionPool*&);
	inline void createPool(QuestionPool*& qpool) {
		if (qpool != nullptr) {
			delete std::exchange(qpool,nullptr);
		}
		qpool = new qna::QuestionPool();
	}
	void addQuestion(QuestionPool*&);
	void addAnswer(QuestionPool*&);
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
	void findMostAnsweredQuestion(QuestionPool*&);
	void findQuestionWithHighestVotedResponse(QuestionPool*&);
	void upvoteResponse(QuestionPool*&);
	Post::PostPrinter::EPrintMode promptForPrintMode();
	void printQuestion(QuestionPool*&);
	void deleteQuestion(QuestionPool*&);
	void deleteResponse(QuestionPool*&);
	void initQuestionPool(QuestionPool*& pool);
}
