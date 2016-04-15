#ifndef _CYCLIC_QUEUE_HPP_
#define _CYCLIC_QUEUE_HPP_

// includes
#include <memory>
#include <list>

// forward declarations
class MovableObject;

class CyclicQueue : private std::list<std::shared_ptr<MovableObject>> {
public:
	typedef std::list<std::shared_ptr<MovableObject>> list;
	using list::list;
	using list::empty;
	void Begin();
	void Begin(iterator beggining);
	inline std::shared_ptr<MovableObject> Get() { return (position_ != end() ? *position_ : nullptr); };
	bool Next();
	/*< Insortion means "insertion sort" or "insert with sorting". It provides
	inserting elements with keeping list sorted. */
	void Insortion (std::shared_ptr<MovableObject> new_object);
	void Remove();
	void Sort();
	inline bool IsEnd() const { return end_; };
private:
//	iterator previous_position_;
	iterator position_;
	bool end_;
//	mutable iterator pushed_end_;
//	CyclicQueue(const CyclicQueue &object);
};

#endif // _CYCLIC_QUEUE_HPP_
