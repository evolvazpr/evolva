#include "CyclicQueue.hpp"

bool CyclicQueue::Next() {
	previous_position_ = position_++;
	end_ = false;
	while (!empty()) {
		if (position_ == end()) {
			position_ = begin();
			end_ = true;
		}
		if ((*position_)->dead_) Remove();
		else return end_;
	}
	return end_;
}

void CyclicQueue::Begin() {
	position_ = begin();
}

void CyclicQueue::Begin(CyclicQueue::iterator beggining) {
	splice_after(before_begin(), *this, beggining, end());
	Begin();
}

void CyclicQueue::Insortion(std::shared_ptr<Unit> new_element) {
	iterator j = before_begin();
	for (iterator i = begin(); i != end(); ++i) {
		if (!new_element->operator <= (**i)) {
			insert_after(j, new_element);
			return;
		}
		j = i;
	}
	insert_after(j, new_element);
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
		std::shared_ptr<Unit> unit = *position_;
		queue.Insortion(unit);
	} while(!Next());
	clear();
	operator = (queue);
}
