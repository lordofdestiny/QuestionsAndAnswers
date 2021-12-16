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
		bool addAnswer(GlobalID::IDType, std::string);
		bool addAnswer(std::string, std::string);
		Answer findQuestion(std::string);
		Answer findQuestion(GlobalID::IDType);
		Answer findHighestVotedResponse(std::string);
		Answer findHighestVotedResponse(GlobalID::IDType);
		bool upvote(GlobalID::IDType);
		bool upvote(std::string);
		void sortAll();
		void printQuestions(std::ostream & = std::cout);
		enum class EPrintMode { LevelOrder, PreOrder };
		template<typename T>
		void printQuestion(T key,
			Post::NodePrinter::EPrintMode mode
			= Post::NodePrinter::EPrintMode::LevelOrder,
			std::ostream& os = std::cout) {
			Post* node = findQuestion(key);
			if (node == nullptr) {
				os << "Question not found!\n";
				return;
			}
			os << Post::NodePrinter(node, mode) << "\n";
		}
		bool deleteQuestion(std::string);
		bool deleteQuestion(GlobalID::IDType);
		bool deleteResponse(std::string);
		bool deleteResponse(GlobalID::IDType);

		std::size_t questionCount() const {
			return _questions.size();
		}
	private:
		bool addAnswerReal(Post* question, std::string& text);
		enum class ESearchType { Any, Question, Response };
		static auto makeTextCmp(std::string& text) {
			return [&text](Post* node) {
				return node->text() == text;
			};
		}
		static auto makeIdCmp(GlobalID::IDType id) {
			return [id](Post* node) {
				return node->id() == id;
			};
		}
		Post* findReal(SearchLambda, ESearchType = ESearchType::Any);
		Post* findResponse(std::string);
		Post* findResponse(GlobalID::IDType);
		Post* findAny(std::string);
		Post* findAny(GlobalID::IDType);
		Post* findHighestVotedInTree(Post*);
		bool upvoteReal(Post*);
		bool deleteQuestionReal(SearchLambda);
		template<typename T>
		bool deleteResponseReal(T parameter, SearchLambda condition) {
			Post* node = findResponse(parameter);
			if (node == nullptr || node->parent() == nullptr) return false;
			auto& pAnswers = node->parent()->answers();
			auto toDelete = std::find_if(pAnswers.begin(), pAnswers.end(), condition);
			if (toDelete == pAnswers.end()) return false;
			pAnswers.erase(toDelete);
			return true;
		}
	};
}
