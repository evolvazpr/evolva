#include "CyclicQueue.hpp"
#include "CellObject.hpp"

bool CyclicQueue::Next() {
	previous_position_ = position_++;
	end_ = false;
	while (!empty()) {
		if (position_ == end()) {
			position_ = begin();
			end_ = true;
		}
		if ((*position_)->IsRemoved()) Remove();
		else return end_;
	}
	return end_;
}

void CyclicQueue::Begin() {
	position_ = begin();
	end_ = false;
}

void CyclicQueue::Begin(CyclicQueue::iterator beggining) {
	splice_after(before_begin(), *this, beggining, end());
	Begin();
}

void CyclicQueue::Insortion(std::shared_ptr<MovableObject> new_object) {
	iterator j = before_begin();
	for (iterator i = begin(); i != end(); ++i) {
		if (!new_object->operator <= (**i)) {
			insert_after(j, new_object);
			return;
		}
		j = i;
	}
	insert_after(j, new_object);
}

void CyclicQueue::Remove() {
	if (end_) {
		erase_after(before_begin());
		position_ = begin();
	}
	else {
		erase_after(previous_position_);
		position_ = previous_position_;
		++position_;
	}
}

void CyclicQueue::Sort() {
	if(empty()) return;
	CyclicQueue queue;
	Begin();
	do {
		std::shared_ptr<MovableObject> object = *position_;
		queue.Insortion(object);
	} while(!Next());
	clear();
	operator = (queue);
}
