#ifndef _CYCLIC_QUEUE_HPP_
#define _CYCLIC_QUEUE_HPP_

// includes
#include <memory>
#include <forward_list>

// forward declarations
class MovableObject;

class CyclicQueue : private std::forward_list<std::shared_ptr<MovableObject>> {
public:
	using std::forward_list<std::shared_ptr<MovableObject>>::forward_list;
	using std::forward_list<std::shared_ptr<MovableObject>>::empty;
	void Begin();
	void Begin(iterator beggining);
	inline std::shared_ptr<MovableObject> Get() { return *position_; };
	bool Next();
	/*< Insortion means "insertion sort" or "insert with sorting". It provides
	inserting elements with keeping list sorted. */
	void Insortion (std::shared_ptr<MovableObject> new_object);
	void Remove();
	void Sort();
	inline bool IsEnd() const { return end_; };
private:
	iterator previous_position_;
	iterator position_;
	bool end_;
	mutable iterator pushed_end_;
};

#endif // _CYCLIC_QUEUE_HPP_
