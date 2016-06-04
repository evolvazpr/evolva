#ifndef _CYCLIC_QUEUE_HPP_
#define _CYCLIC_QUEUE_HPP_

// includes
#include <memory>
#include <list>

// forward declarations
class Unit;

/*< Cyclic queue provides simple priortiy queue in which
priority is some speed ability of units. Cyclic queue
returns to the beggining of queue if it reaches end.*/
class CyclicQueue : private std::list<std::shared_ptr<Unit>> {
public:
	typedef std::list<std::shared_ptr<Unit>> list;
	using list::list;
	using list::empty;
	/*< Begin sets the beggining od cyclic queue. */
	void Begin();
	/*< Returns current unit or nullptr if queue is empty. */
	inline std::shared_ptr<Unit> Get() { return (position_ != end() ? *position_ : nullptr); };
	/*< Take next step in queue. */
	bool Next();
	/*< Insortion means "insertion sort" or "insert with sorting". It provides
	inserting elements with keeping list sorted. If keep_position is set it
	remembers last current position and after Insortion it restores that.
	Also, if Next() meets a dead unit, method removes this from queue.*/
	void Insortion (std::shared_ptr<Unit> new_object, const bool keep_position = true);
	/*< Removes CURRENT element from the queue. */
	void Remove();
	/*< Sorts queue. It works like insertion sort with new empty queue using
	provided Insortion method. After sorting, the new beggining is set.*/
	void Sort();
	/*< Returns is that end (beggining) of loop. */
	inline bool IsEnd() const { return end_; };
private:
	iterator position_;
	bool end_;
};

#endif // _CYCLIC_QUEUE_HPP_
