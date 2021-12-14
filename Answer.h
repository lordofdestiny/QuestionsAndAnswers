#pragma once
#include "Post.h"

namespace qna {
	class Post;
	class Answer {
		qna::Post* _ptr;
		int _rating = 0;
	public:
		Answer(Post* node = nullptr) :_ptr(node) {}
		Post* operator->() {
			return _ptr;
		}
		Post const* operator->() const {
			return _ptr;
		}
		Post& operator*() {
			return *_ptr;
		}
		Post const& operator*() const {
			return *_ptr;
		}
		operator qna::Post* () const {
			return _ptr;
		}
		int votes() const {
			return _rating;
		}
		int upvote() {
			return ++_rating;
		}
		bool operator<(Answer const& that)const {
			return _rating < that._rating;
		}
		bool operator>(Answer const& that)const {
			return _rating > that._rating;
		}
	};
}