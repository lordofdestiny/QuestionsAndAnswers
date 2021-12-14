#include <iostream>
#include "QuestionPool.h"
#include "Helpers.h"
#include "ControlFunctions.h"
#define PRE_INIT 1

// i == 1 , j == 1
// level order
// pronalazak odgovora ili komentara na zadato pitanje sa najvisom ocenom

int main()
{
	using qna::QuestionPool;
#if PRE_INIT == 0
	QuestionPool* mainPool = nullptr;
#else
	QuestionPool* mainPool = new QuestionPool();
	mainPool->addQuestion("Question 1");
	mainPool->addAnswer("Question 1", "Answer 1");
	mainPool->addAnswer("Question 1", "Answer 2");
	mainPool->addAnswer("Answer 1", "Answer 1.1");
	mainPool->addAnswer("Answer 1", "Answer 1.2");
	mainPool->addAnswer("Answer 1.1", "Answer 1.1.1");
	mainPool->addAnswer("Answer 1.1", "Answer 1.1.2");
	mainPool->addAnswer("Answer 1.1", "Answer 1.1.3");
	mainPool->addAnswer("Answer 2", "Answer 2.1");
	mainPool->addAnswer("Answer 2", "Answer 2.2");
	mainPool->addAnswer("Answer 2.2", "Answer 2.2.1");
	mainPool->addAnswer("Answer 2.2", "Answer 2.2.2");
	mainPool->addAnswer("Answer 2.2", "Answer 2.2.3");
	mainPool->addQuestion("Question 2");
	mainPool->addAnswer("Question 2","Answer X");
	mainPool->addAnswer("Question 2","Answer XX");
	mainPool->addAnswer("Question 2","Answer XXX");
	mainPool->addQuestion("Question 3");
	mainPool->addAnswer("Question 3", "Only this answer");
	mainPool->upvote("Answer 1.1.1");
	mainPool->upvote("Answer 1.1.1");
	mainPool->upvote("Answer 1.1.2");
	mainPool->upvote("Answer 1.1.3");
	mainPool->upvote("Answer 1.1.3");
	mainPool->upvote("Answer 1.1.3");
	mainPool->upvote("Answer 2.1");
	mainPool->upvote("Answer 2.2");
	mainPool->upvote("Answer 2");
#endif
	int code = 1;
	cnt::printMenu();
	std::cout << cnt::separator;
	while (true) {
		if (code != 0) {
			if (mainPool == nullptr) {
				std::cout << "First initialize the question pool! "
					"Please select option 1.\nAll actions are ignored before"
					" initialization\n" << cnt::separator;
			}
		}
		else {
			std::cout << "Wrong choice, pick again!\n";
		}
		int choice;
		std::cout << "Enter your choice: ";
		std::cin >> choice;
		code = cnt::activateChoice(choice, mainPool);
		std::cout << cnt::separator;
		if (code == -1) {
			std::cout << "Exiting program... Your memory will be cleaned up.\n";
			break;
		}
	}
	delete mainPool;
}
