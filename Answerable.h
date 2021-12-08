#pragma once
#include <array>
#include "Comment.h"

namespace qna {
	template<class AnswerType, unsigned maxAnswers = 10>
	class Answerable :
		public AnswerType
	{
	public:
		friend class QuestionPool;
		using Answer = Answerable<AnswerType>;
		using Question = Answerable<Answer>;
	public:
		Answerable() : AnswerType() {
			AnswerType::_type = deduceType();
		}
		Answerable(std::string answer) :AnswerType(answer) {
			AnswerType::_type = deduceType();
		}
		Answerable(Answerable const&) = default;
		Answerable(Answerable&& rhs) noexcept :
			AnswerType(std::move(rhs._text)),
			_answerCount(rhs._answerCount),
			_answers(std::move(_answers)) {}
		Answerable& operator=(Answerable const&) = default;
		Answerable& operator=(Answerable&& other) noexcept {
			if (this != &other) {
				AnswerType::_text = std::move(other._text);
				_answerCount = other._answerCount;
				_answers = other._answers;
			}
			return *this;
		}
		std::array<AnswerType, maxAnswers>& answers() {
			return _answers;
		}
	private:
		static constexpr EAType deduceType() {
			if (std::is_same_v<AnswerType, Answer>) {
				return EAType::Answer;
			}
			else if (std::is_same_v<AnswerType, Question>) {
				return EAType::Question;
			}
			else {
				return EAType::NotAnswer;
			}
		}
		unsigned _answerCount = 0;
		std::array<AnswerType,maxAnswers> _answers;
	};
	using Answer = Answerable<Comment>::Answer;
	using Question = Answerable<Comment>::Question;
}