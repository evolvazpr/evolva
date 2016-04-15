#include "CyclicQueue.hpp"
#include "CellObject.hpp"

bool CyclicQueue::Next() {
	if (empty()) return (end_ = true); // ?
	++position_;
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
//	splice_after(before_begin(), *this, beggining, end());
	Begin();
}

#include <iostream>

void CyclicQueue::Insortion(std::shared_ptr<MovableObject> new_object) {
	std::cout << "cq size bf: " << size() << "\n";
	if (empty()) {
		push_back(new_object);
		position_ = begin();
		std::cout << "cq size af: " << size() << "\n";
		return;
	}
	bool inserted = false;
	for (iterator i = begin(); i != end(); ++i) {
		if (!new_object->operator <= (**i)) {
			insert(i, new_object);
			inserted = true;
			break;
		}
	}
	if (!inserted) push_back(new_object);
	std::cout << "cq size af: " << size() << "\n";
	auto address = &(*position_);
	position_ = begin();
	int i = 0;
	for (; ; ++i) {
		if (position_ == end()) break;
		else ++position_;
	}
	position_ = begin();
	std::cout << "real size: " << i << "\n";
	while (1) {
		auto b = &(*position_);
		if (b != address) ++position_;
		else break;
	}

/*
	iterator j = before_begin();
	for (iterator i = begin(); i != end(); ++i) {
		if (!new_object->operator <= (**i)) {
			if (j == position_) {
				insert(j, new_object);
				++(--position_);
			}
	/*		else if (j == previous_position_) {
				previous_position_ = insert_after(j, new_object);
			}
			else /**insert_after(j, new_object);
			return;
		}
		j = i;
	}
	insert_after(j, new_object); // dobrze?/**/
}

void CyclicQueue::Remove() {
	if (end_) {
		erase(begin());
		position_ = begin();
	}
	else {
		auto previous_position = position_;
		--previous_position;
		erase(position_);
		position_ = previous_position;
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
/*
CyclicQueue::CyclicQueue(const CyclicQueue &object) : list(object) {
	end_ = object.end_;
	std::advance(previous_position_, std::distance(object.begin(), object.previous_position_);
	std::advance(position_, std::distance(object.begin(), object.position_);
	std::advance(pushed_end_, std::distance(object.begin(), object.pushed_end_);
}
/**/
