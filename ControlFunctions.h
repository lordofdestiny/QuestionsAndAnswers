#pragma once
#include <iostream>
#include "QuestionPool.h"

namespace cnt {
	using namespace qna;
	void printMenu();
	template<typename F>
	void textOrIdMenuHandler(F handle) {
		bool loop = true;
		while (loop) {
			std::cout << "Search by:\n";
			std::cout << "1. Post text\n";
			std::cout << "2. Post ID\n";
			std::cout << "3. Cancel\n";
			std::cout << "Pick an option: ";
			int option;
			std::cin >> option;
			if (option == 3) {
				std::cout << "Command cancelled\n";
				break;
			}
			if (option != 1 && option != 2) {
				std::cout << "Wrong option! Try again\n";
				continue;
			}
			handle(option);
			loop = false;
		}
	}
	int activateChoice(int, QuestionPool*&);
	inline void createPool(QuestionPool*& qpool) {
		if (qpool != nullptr) {
			delete qpool;
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
	void printQuestion(QuestionPool*&);
	void deleteQuestion(QuestionPool*&);
	void deleteResponse(QuestionPool*&);
	void initQuestionPool(QuestionPool*& pool);
}