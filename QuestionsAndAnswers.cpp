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
	QuestionPool* mainPool = nullptr;
#if PRE_INIT
	cnt::initQuestionPool(mainPool);
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
		auto choice = cnt::promptValue<int>("Enter your choice : ");
		code = cnt::activateChoice(choice, mainPool);
		std::cout << cnt::separator;
		if (code == -1) {
			std::cout << "Exiting program... Your memory will be cleaned up.\n";
			break;
		}
	}
	delete mainPool;
}
