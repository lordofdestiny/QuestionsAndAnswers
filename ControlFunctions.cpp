#include "ControlFunctions.h"
#include "Helpers.h"

namespace cnt {

	void cnt::textOrIdMenuHandler(std::function<void(int)> handle) {
		bool loop = true;
		while (loop) {
			std::cout << "Search by:\n";
			std::cout << "1. Post text\n";
			std::cout << "2. Post ID\n";
			std::cout << "3. Cancel\n";
			int option = promptValue<int>("Pick an option: ");
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

	void cnt::printMenu() {
		std::cout << cnt::separator;
		std::cout << "Menu: \n";
		std::cout << "0. Print menu\n";
		std::cout << "1. Create QuestionPool(Recreate if it exists)\n";
		std::cout << "2. Add new question\n";
		std::cout << "3. Add a response\n";
		std::cout << "4. Search questions\n";
		std::cout << "5. Search higest rated for question\n";
		std::cout << "6. Search most answered question\n";
		std::cout << "7. Find question which has highest rated response\n";
		std::cout << "8. Upvote an answer\n";
		std::cout << "9. Sort answers\n";
		std::cout << "10. Print all questions\n";
		std::cout << "11. Print a question in detail\n";
		std::cout << "12. Delete a question\n";
		std::cout << "13. Delete a response\n";
		std::cout << "14. Exit\n";
	}

	void initQuestionPool(qna::QuestionPool*& qpool) {
		qpool = new QuestionPool();
		qpool->addQuestion("Question 1");
		qpool->addAnswer<std::string>("Question 1", "Answer 1");
		qpool->addAnswer<std::string>("Question 1", "Answer 2");
		qpool->addAnswer<std::string>("Answer 1", "Answer 1.1");
		qpool->addAnswer<std::string>("Answer 1", "Answer 1.2");
		qpool->addAnswer<std::string>("Answer 1.1", "Answer 1.1.1");
		qpool->addAnswer<std::string>("Answer 1.1", "Answer 1.1.2");
		qpool->addAnswer<std::string>("Answer 1.1", "Answer 1.1.3");
		qpool->addAnswer<std::string>("Answer 2", "Answer 2.1");
		qpool->addAnswer<std::string>("Answer 2", "Answer 2.2");
		qpool->addAnswer<std::string>("Answer 2.2", "Answer 2.2.1");
		qpool->addAnswer<std::string>("Answer 2.2", "Answer 2.2.2");
		qpool->addAnswer<std::string>("Answer 2.2", "Answer 2.2.3");
		qpool->addQuestion("Question 2");
		qpool->addAnswer<std::string>("Question 2", "Answer X");
		qpool->addAnswer<std::string>("Question 2", "Answer XX");
		qpool->addAnswer<std::string>("Question 2", "Answer XXX");
		qpool->addQuestion("Question 3");
		qpool->addAnswer<std::string>("Question 3", "Only this answer");
		qpool->upvote<std::string>("Answer 1.1.1");
		qpool->upvote<std::string>("Answer 1.1.1");
		qpool->upvote<std::string>("Answer 1.1.2");
		qpool->upvote<std::string>("Answer 1.1.3");
		qpool->upvote<std::string>("Answer 1.1.3");
		qpool->upvote<std::string>("Answer 1.1.3");
		qpool->upvote<std::string>("Answer 2.1");
		qpool->upvote<std::string>("Answer 2.2");
		qpool->upvote<std::string>("Answer 2");
	}

	int activateChoice(int choice, qna::QuestionPool*& qpool) {
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
			cnt::findMostAnsweredQuestion(qpool);
			break;
		case 7:
			cnt::findQuestionWithHighestVotedResponse(qpool);
			break;
		case 8:
			cnt::upvoteResponse(qpool);
			break;
		case 9:
			qpool->sortAll();
			std::cout << "All trees were sorted!\n";
			break;
		case 10:
			cnt::printQuestions(qpool);
			break;
		case 11:
			cnt::printQuestion(qpool);
			break;
		case 12:
			cnt::deleteQuestion(qpool);
			break;
		case 13:
			cnt::deleteResponse(qpool);
			break;
		case 14:
			return -1;
		default:
			return 0;
		}
		return 1;
	}

	void addQuestion(qna::QuestionPool*& qpool) {
		std::string str = promptValue<std::string>("Question text: ");
		qpool->addQuestion(str);
		std::cout << "Question added!\n";
	}

	void addAnswer(qna::QuestionPool*& qpool) {
		textOrIdMenuHandler([qpool](int option) {
			bool result = false;
			if (option == 1) {
				auto text = promptValue<std::string>("Enter text: ");
				std::string answerText = promptValue<std::string>("Answer text: ");
				result = qpool->addAnswer(text, text);
			}
			else {
				auto id = promptValue<GlobalID::IDType>("Enter id: ");
				std::string answerText = promptValue<std::string>("Answer text: ");
				result = qpool->addAnswer(id, answerText);
			}
			std::cout << (result ? "Answer added!\n" : "No entery found!\n");
			});
	}

	void searchQuestions(qna::QuestionPool*& qpool) {
		textOrIdMenuHandler([qpool](int option) {
			Post* result;
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

	void findHighestRatedResponse(qna::QuestionPool*& qpool) {
		textOrIdMenuHandler([qpool](int option) {
			Post* result;
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

	void upvoteResponse(qna::QuestionPool*& qpool) {
		textOrIdMenuHandler([qpool](int option) {
			bool result = false;
			if (option == 1) {
				std::string question = promptValue<std::string>("Response text: ");
				result = qpool->upvote(question);
			}
			else {
				auto id = promptValue<GlobalID::IDType>("Response id: ");
				result = qpool->upvote(id);
			}
			std::cout << (result ? "Response upvoted!\n" : "No response found!\n");
			});
	}

	Post::PostPrinter::EPrintMode promptForPrintMode() {
		using enum Post::PostPrinter::EPrintMode;
		bool loop = true;
		while (loop) {
			std::cout << "Output type: \n";
			std::cout << "1. Question only\n";
			std::cout << "2. Preorder\n";
			std::cout << "3. Inorder\n";
			std::cout << "4. Postorder\n";
			std::cout << "5. Level order\n";
			int option = promptValue<int>("Pick an option: ");
			if (option < 0 || option > 5) {
				std::cout << "Wrong option! Try again\n";
				continue;
			}
			return static_cast<Post::PostPrinter::EPrintMode>(option - 1);
			loop = false;
		}
		return Root;
	}

	void printQuestion(qna::QuestionPool*& qpool) {
		textOrIdMenuHandler([qpool](int option) {
			if (option == 1) {
				auto question = promptValue<std::string>("Question text: ");
				qpool->printQuestion(question, promptForPrintMode());
			}
			else {
				auto id = promptValue<GlobalID::IDType>("Question id: ");
				qpool->printQuestion(id, promptForPrintMode());
			}
			});
	}

	void deleteQuestion(qna::QuestionPool*& qpool) {
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
			else {
				auto id = promptValue<GlobalID::IDType>("Question id: ");
				result = qpool->deleteQuestion(id);
			}
			std::cout << (result
				? "Question deleted!\n"
				: "No question was found!\n");
			});
	}

	void deleteResponse(qna::QuestionPool*& qpool) {
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

	void findMostAnsweredQuestion(QuestionPool*& qpool) {
		auto question = qpool->findMostAnsweredQuestion();
		if (question == nullptr) {
			std::cout << "There were no questions to search from!\n";
		}
		else {
			std::cout << question->asPreOrder() << '\n';
		}
	}

	void findQuestionWithHighestVotedResponse(QuestionPool*& qpool) {
		auto question = qpool->findQuestionWithHighestVotedResponse();
		if (question == nullptr) {
			std::cout << "There were no questions to search from!\n";
		}
		else {
			std::cout << question->asPreOrder() << '\n';
		}
	}

}