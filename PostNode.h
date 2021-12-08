#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include "Post.h"

namespace qna {
	class PostNode
	{
	public:
		friend class QuestionPool;
	public:
		PostNode(std::string const& text, EPostType type = EPostType::Comment, PostNode* parent = nullptr)
			: _post(text, type),
			_parent(parent) {}
		PostNode(PostNode const&) = default;
		PostNode(PostNode&& rhs) noexcept :
			_post(std::move(rhs._post)),
			_parent(rhs._parent),
			_answers(std::move(rhs._answers)) {}
		PostNode& operator=(PostNode const& other) = default;
		PostNode& operator=(PostNode&&) noexcept;
		Post& post() {
			return _post;
		}
		Post const& post() const {
			return _post;
		}
		std::vector<qna::PostNode>& answers() {
			return _answers;
		}
		std::vector<qna::PostNode> const& answers() const {
			return _answers;
		}
		PostNode* parent() const {
			return _parent;
		}
		unsigned answerCount() const {
			return static_cast<unsigned>(_answers.size());
		}
		auto begin() {
			return _answers.begin();
		}
		auto end() {
			return _answers.end();
		}
		void sortAnswers();
		bool answerPost(std::string const&, EPostType);
		friend std::ostream& operator<<(std::ostream& os, PostNode const& node) {
			EPostType type = node.post().type();
			std::string typeStr =
				type == EPostType::Question
				? "Q" : type == EPostType::Answer
				? "A" : "C";
			std::string parent = type == EPostType::Question
				? "R" : std::to_string(node.parent()->post().id());
			os << typeStr << "(" << parent << ", ID=" << node.post().id() << "): [\"";
			os << node.post().text() << "\", Ans{" << node.answerCount() << "}";
			if (type != EPostType::Question) {
				os << ", Vote{" << node.post().votes() << "}";
			}
			return os << "]";
		}
	private:
		void copyAnswers(std::vector<qna::PostNode> const& from);
		Post _post;
		PostNode* _parent = nullptr;
		std::vector<qna::PostNode> _answers;
	};
}

