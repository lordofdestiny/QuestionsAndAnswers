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
	std::cout << "10. Delete a question\n";
	std::cout << "11. Delete a response\n";
	std::cout << "12. Exit\n";
}

int cnt::activateChoice(int choice, qna::QuestionPool*& qpool) {
	if (choice != 0 && choice != 1 && choice != 12 && qpool == nullptr) return 1;
	switch (choice)
	{
	case 0:
		cnt::printMenu();
		break;
	case 1:
		cnt::createPool(qpool);
		break;
	case 2:
		cnt::addQuestion(qpool);
		break;
	case 3:
		cnt::addAnswer(qpool);
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
		qpool->sortAll();
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
	std::string str = promptValue<std::string>("Question text: ");
	qpool->addQuestion(str);
	std::cout << "Question added!\n";
}

void cnt::addAnswer(qna::QuestionPool*& qpool) {
	cnt::textOrIdMenuHandler([qpool](int option) {
		bool result = false;
		if (option == 1) {
			auto text = promptValue<std::string>("Enter text: ");
			std::string answerText = promptValue<std::string>("Answer text: ");
			result = qpool->postAnswer(text, text);
		}
		else {
			auto id = promptValue<GlobalID::IDType>("Enter id: ");
			std::string answerText = promptValue<std::string>("Answer text: ");
			result = qpool->postAnswer(id, answerText);
		}
		std::cout << (result ? "Answer added!\n" : "No entery found!\n");
		});
}

void cnt::searchQuestions(qna::QuestionPool*& qpool) {
	cnt::textOrIdMenuHandler([qpool](int option) {
		Answer result;
		if (option == 1) {
			auto question = promptValue<std::string>("Question text: ");
			result = qpool->findQuestion(question);
		}
		else {
			auto id = promptValue<GlobalID::IDType>("Question id: ");
			result = qpool->findQuestion(id);
		}
		std::cout << (result != nullptr
			? "Question found!\n"
			: "No question found!\n");
		});
}

void cnt::findHighestRatedResponse(qna::QuestionPool*& qpool) {
	cnt::textOrIdMenuHandler([qpool](int option) {
		Answer result;
		if (option == 1) {
			auto question = promptValue<std::string>("Enter question text: ");
			result = qpool->findHighestVotedResponse(question);
		}
		else {
			auto id = promptValue<GlobalID::IDType>("Enter question id: ");
			result = qpool->findHighestVotedResponse(id);
		}
		if (result != nullptr) {
			std::cout << *result << "\n";
		}
		else {
			std::cout << "No matching answers were found!\n";
		}
		});
}

void cnt::upvoteResponse(qna::QuestionPool*& qpool) {
	cnt::textOrIdMenuHandler([qpool](int option) {
		bool result = false;
		if (option == 1) {
			std::string question = promptValue<std::string>("Response text: ");
			result = qpool->upvote(question);
		}
		else {
			unsigned long id = promptValue<GlobalID::IDType>("Response id: ");
			result = qpool->upvote(id);
		}
		std::cout << (result ? "Response upvoted!\n" : "No response found!\n");
		});
}

void cnt::printQuestion(qna::QuestionPool*& qpool) {
	textOrIdMenuHandler([qpool](int option) {
		if (option == 1) {
			auto question = promptValue<std::string>("Question text: ");
			qpool->printQuestion(question);
		}
		else {
			auto id = promptValue<GlobalID::IDType>("Question id: ");
			qpool->printQuestion(id);
		}
		});
}

void cnt::deleteQuestion(qna::QuestionPool*& qpool) {
	textOrIdMenuHandler([qpool](int option) {
		if (qpool->questionCount() <= 0) {
			std::cout << "There are no questions to delete!\n";
			return;
		}
		bool result;
		if (option == 1) {
			auto question = promptValue<std::string>("Question text: ");
			result = qpool->deleteQuestion(question);
		}
		else {//must be 2 
			auto id = promptValue<GlobalID::IDType>("Question id:");
			result = qpool->deleteQuestion(id);
		}
		std::cout << (result
			? "Question deleted!\n"
			: "No question was found!\n");
		});
}

void cnt::deleteResponse(qna::QuestionPool*& qpool) {
	textOrIdMenuHandler([qpool](int option) {
		if (qpool->questionCount() <= 0) {
			std::cout << "There are no questions to delete responses from!\n";
			return;
		}
		bool result = false;
		if (option == 1) {
			auto id = promptValue<std::string>("Response text: ");
			result = qpool->deleteResponse(id);
		}
		else {
			auto id = promptValue<GlobalID::IDType>("Response id: ");
			result = qpool->deleteResponse(id);
		}
		std::cout << (result
			? "Answer deleted!\n"
			: "There was no answer to delete!\n");
		});
}