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
			: _post(text, type), _parent(parent) {}
		PostNode(PostNode const& rhs) :
			_post(rhs._post) {
			copyResponses(rhs._answers);
		}
		PostNode(PostNode&& rhs) noexcept :
			_post(std::move(rhs._post)),
			_parent(rhs._parent),
			_answers(std::move(rhs._answers)) {}
		PostNode& operator=(PostNode const& other) {
			_post = other._post;
			_answers.clear();
			copyResponses(other._answers);
		};
		PostNode& operator=(PostNode&&) noexcept;
		~PostNode() {
			freeResponses();
		}
		Post& post() {
			return _post;
		}
		Post const& post() const {
			return _post;
		}
		auto& answers() {
			return _answers;
		}
		auto const& answers() const {
			return _answers;
		}
		PostNode* parent() const {
			return _parent;
		}
		unsigned answerCount() const {
			return static_cast<unsigned>(_answers.size());
		}
		void sortResponses();
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
		void copyResponses(std::vector<qna::PostNode*> const& from);
		void freeResponses();
		Post _post;
		PostNode* _parent = nullptr;
		std::vector<qna::PostNode*> _answers;
	};
}