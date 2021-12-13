#pragma once
#include <atomic>
namespace qna {
	class GlobalID
	{
	public:
		using IDType = unsigned long;
	private:
		static inline std::atomic<IDType> _nextID = 0;
		IDType _id;
	public:
		GlobalID() : _id(_nextID++) {}
		GlobalID(GlobalID const& rhs) = default;
		GlobalID& operator=(GlobalID const&) = default;
		operator IDType() const {
			return _id;
		}
		bool operator==(GlobalID const& other) {
			return _id == other._id;
		}
		bool operator!=(GlobalID const& other) {
			return _id != other._id;
		}
	};
}

