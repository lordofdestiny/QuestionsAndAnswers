#include "ControlFunctions.h"
#include "Helpers.h"

void cnt::printMenu() {
	std::cout << cnt::separator;
	std::cout << "Menu: \n";
	std::cout << "0. Print menu\n";
	std::cout << "1. Create QuestionPool(Recreate if it exists)\n";
	std::cout << "2. Add new question\n";
	std::cout << "3. Add a response\n";
	std::cout << "4. Search questions\n";
	std::cout << "5. Search higest rated for question\n";
	std::cout << "6. Upvote an answer\n";
	std::cout << "7. Sort answers\n";
	std::cout << "8. Print all questions\n";
	std::cout << "9. Print a question in detail\n";
	std::cout << "10. Delete an answer\n";
	std::cout << "11. Delete a question\n";
	std::cout << "12. Exit\n";
}

int cnt::activateChoice(int choice, qna::QuestionPool*& qpool) {
	if (choice != 0 && choice != 1 && choice != 12 && qpool == nullptr) return 1;
	switch (choice)
	{
	case 0:
		cnt::printMenu();
		std::cout << cnt::separator;
		break;
	case 1:
		cnt::createPool(qpool);
		break;
	case 2:
		cnt::addQuestion(qpool);
		break;
	case 3:
		cnt::answerQuestionOrResponse(qpool);
		break;
	case 4:
		cnt::searchQuestions(qpool);
		break;
	case 5:
		cnt::findHighestRatedResponse(qpool);
		break;
	case 6:
		cnt::upvoteResponse(qpool);
		break;
	case 7:
		qpool->sortAnswers();
		std::cout << "All trees were sorted!\n";
		break;
	case 8:
		cnt::printQuestions(qpool);
		break;
	case 9:
		cnt::printQuestion(qpool);
		break;
	case 10:
		cnt::deleteQuestion(qpool);
		break;
	case 11:
		cnt::deleteResponse(qpool);
		break;
	case 12:
		return -1;
	default:
		return 0;
	}
	return 1;
}

void cnt::addQuestion(qna::QuestionPool*& qpool) {
	std::cout << "Enter question text: ";
	std::string str;
	std::cin.ignore();
	std::getline(std::cin, str);
	qpool->addQuestion(str);
	std::cout << "Question successfully added!\n";
}

void cnt::answerQuestionOrResponse(qna::QuestionPool*& qpool) {
	bool loop = true;
	while (loop) {
		unsigned long id;
		std::cout << "Enter id: ";
		std::cin >> id;
		PostNode* node = qpool->findAnyById(id);
		std::string text;
		std::cout << "Enter text for the answer: ";
		std::cin.ignore();
		std::getline(std::cin, text);
		std::cout << (qpool->answer(node, text)
			? "Answer successfully added!\n" :
			"This question was not found! Check input and try again!\n");
		loop = false;
	}
}

void cnt::searchQuestions(qna::QuestionPool*& qpool) {
	bool loop = true;
	while (loop) {
		std::cout << "Search options:\n";
		std::cout << "1. By exact question text\n";
		std::cout << "2. By question ID\n";
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
		qna::PostNode* result;
		if (option == 1) {
			std::string question;
			std::cout << "Enter question text: ";
			std::cin.ignore();
			std::getline(std::cin, question);
			result = qpool->findQuestion(question);
		}
		else {//must be 2 
			unsigned long id;
			std::cout << "Enter question id: ";
			std::cin >> id;
			result = qpool->findQuestion(id);
		}
		std::cout << (result != nullptr
			? "Question found!\n"
			: "No matching question was found!\n");
		loop = false;
	}
}

void cnt::findHighestRatedResponse(qna::QuestionPool*& qpool) {
	bool loop = true;
	while (loop) {
		std::cout << "Search options:\n";
		std::cout << "1. By exact question text\n";
		std::cout << "2. By question ID\n";
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
		qna::PostNode* result;
		if (option == 1) {
			std::string question;
			std::cout << "Enter question text: ";
			std::cin.ignore();
			std::getline(std::cin, question);
			result = qpool->findHighestVotedResponse(question);
		}
		else {//must be 2 
			unsigned long id;
			std::cout << "Enter question id: ";
			std::cin >> id;
			result = qpool->findHighestVotedResponse(id);
		}
		std::cout << (result != nullptr
			? "Most voted answer found!\n"
			: "No matching answers were found!\n");
		if (result) {
			qna::QuestionPool::printTree(result);
		}
		loop = false;
	}
}

void cnt::upvoteResponse(qna::QuestionPool*& qpool) {
	std::cout << "Enter response id: ";
	unsigned long id;
	std::cin >> id;
	std::cout << (qpool->upvote(id)
		? "Answer with given id upvoted!\n"
		: "There was no question with given ID!\n");
}

void cnt::printQuestion(qna::QuestionPool*& qpool) {
	bool loop = true;
	while (loop) {
		std::cout << "Search options:\n";
		std::cout << "1. By exact question text\n";
		std::cout << "2. By question ID\n";
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
		qna::PostNode* result;
		if (option == 1) {
			std::string question;
			std::cout << "Enter question text: ";
			std::cin.ignore();
			std::getline(std::cin, question);
			result = qpool->findQuestion(question);
		}
		else {//must be 2 
			unsigned long id;
			std::cout << "Enter question id: ";
			std::cin >> id;
			result = qpool->findQuestion(id);
		}
		if (result != nullptr) {
			qna::QuestionPool::printTree(result);
		}
		else {
			std::cout << "No matching question found!\n";
		}
		loop = false;
	}
}

void cnt::deleteQuestion(qna::QuestionPool*& qpool) {
	if (qpool->questionCount() <= 0) {
		std::cout << "There are no questions to delete!\n";
		return;
	}
	bool loop = true;
	while (loop) {
		std::cout << "Search options:\n";
		std::cout << "1. By exact question text\n";
		std::cout << "2. By question ID\n";
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
		bool result;
		if (option == 1) {
			std::string question;
			std::cout << "Enter question text: ";
			std::cin.ignore();
			std::getline(std::cin, question);
			result = qpool->deleteQuestion(question);
		}
		else {//must be 2 
			unsigned long id;
			std::cout << "Enter question id: ";
			std::cin >> id;
			result = qpool->deleteQuestion(id);
		}
		std::cout << (result
			? "Question deleted successfully\n"
			: "No matching question found!\n");
		loop = false;
	}
}

void cnt::deleteResponse(qna::QuestionPool*& qpool) {
	if (qpool->questionCount() <= 0) {
		std::cout << "There are no questions to delete responses from!\n";
		return;
	}
	std::cout << "Enter response id: ";
	unsigned long id;
	std::cin >> id;
	std::cout << (qpool->deleteResponse(id)
		? "Answer with given id deleted!\n"
		: "There was no question with given ID!\n");
}