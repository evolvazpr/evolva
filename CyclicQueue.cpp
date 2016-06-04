#include "CyclicQueue.hpp"
#include "CellObject.hpp"
#include "Unit.hpp"
#include <iostream>
#include "EvolvaException.hpp"
#include <limits>

bool CyclicQueue::Next() {
	if (empty()) {
		return (end_ = true); // ?
	}
	++position_;
	end_ = false;
	while (!empty()) {
		if (position_ == end()) {
			position_ = begin();
			end_ = true;
		}
		if (!(*position_)->IsAlive()) {
			Remove();
		}
		else return end_;
	}
	return end_;
}

void CyclicQueue::Begin() {
	position_ = begin();
	end_ = false;
}

void CyclicQueue::Insortion(std::shared_ptr<Unit> new_object, const bool keep_position) {
	if (empty()) {
		push_back(new_object);
		position_ = begin();
		return;
	}
	size_t position_id = std::numeric_limits<size_t>::max();
	if (keep_position) {
		end_ = false;
		bool empty_queue = false;
		while (!((*position_)->IsAlive())) {
			if (!Next()) {
				empty_queue = true;
				break;
			}
		}
		if (empty_queue) {
			push_back(new_object);
			position_ = begin();
			return;
		}
		position_id = (*position_)->GetId();
	}
	bool inserted = false;
	for (iterator i = begin(); i != end(); ++i) {
		bool condition = false;
		try {
			condition = !new_object->operator <= (*i);
		}
		catch (const EvolvaException &e) {
			//tolog
			std::cout << "\nERROR: Possible lost of cyclic queue data.\n Cyclic queue is corrupted but program will continue.\nDetails: " << e.what() << "\n";
		}
		if (condition) {
			insert(i, new_object);
			inserted = true;
			break;
		}
	}
	if (!inserted) push_back(new_object);
	if (keep_position) {
		Begin();
		if (position_id != std::numeric_limits<size_t>::max()) {
			while ((*position_)->GetId() != position_id) {
				if (!Next()) {
					Begin();
					break;
				}
			}
		}
	}

	/*
	auto address = &(*position_);
	position_ = begin();
	int i = 0;
	for (; ; ++i) {
		if (position_ == end()) break;
		else ++position_;
	}
	position_ = begin();
	while (1) {
		auto b = &(*position_);
		if (b != address) ++position_;
		else break;
	}/**/
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
		std::shared_ptr<Unit> object = *position_;
		queue.Insortion(object, false);
	} while(!Next());
	clear();
	operator = (queue);
	Begin();
}
