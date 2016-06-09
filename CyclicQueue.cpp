#include "CyclicQueue.hpp"
#include "CellObject.hpp"
#include "Unit.hpp"
#include "EvolvaException.hpp"

CyclicQueue::CyclicQueue() : queue_([](std::shared_ptr<Unit> u1, std::shared_ptr<Unit> u2) -> bool { return u1->GetMovePriority() < u2->GetMovePriority(); }) {
	new_cycle_ = true;
}

CyclicQueue::~CyclicQueue() {
}

#include <iostream>

bool CyclicQueue::Next() {
	if (queue_.empty()) {
		return false;
	}
	std::shared_ptr<Unit> unit = queue_.top();
	queue_.pop();
	if (unit->IsAlive()) {
		to_queue_.push_back(unit);
	}
	if (queue_.empty()) {
		new_cycle_ = true;
		Repush();
		if (queue_.empty()) {
			return false;
		}
		else {
			return true;
		}
	}
	else {
		new_cycle_ = false;
		return true;
	}
}

bool CyclicQueue::IsEmpty() const {
	return to_queue_.empty() && queue_.empty();
}

bool CyclicQueue::IsNewCycle() const {
	return new_cycle_;
}

void CyclicQueue::Repush() {
	for (auto i = to_queue_.begin(); i != to_queue_.end(); ++i) {
		queue_.push(*i);
	}
	to_queue_.clear();
}

void CyclicQueue::Insert(std::shared_ptr<Unit> unit) {
	to_queue_.push_back(unit);
}

std::shared_ptr<Unit> CyclicQueue::Get() {
	if(queue_.empty()) {
		return nullptr;
	}
	else {
		return queue_.top();
	}
}

size_t CyclicQueue::GetQueueSize() const {
	return queue_.size();
}

size_t CyclicQueue::GetQueueBufferSize() const {
	return to_queue_.size();
}
