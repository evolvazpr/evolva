#ifndef _CYCLIC_QUEUE_HPP
#define _CYCLIC_QUEUE_HPP

#include "Unit.hpp"
#include <forward_list>

class CyclicQueue : private std::forward_list<std::shared_ptr<Unit>> {
public:
	using std::forward_list<std::shared_ptr<Unit>>::forward_list;
	using empty;
	void Begin();
	void Begin(iterator beggining);
	inline std::shared_ptr<Unit> Get() { return *position_; };
	bool Next();
	/*< Insortion means "insertion sort" or "insert with sorting". It provides
	inserting elements with keeping list sorted. */
	void Insortion (std::shared_ptr<Unit> new_element);
	void Remove();
	void Sort();
private:
	iterator previous_position_;
	iterator position_;
	bool end_;
};

#endif // _CYCLIC_QUEUE_HPP
