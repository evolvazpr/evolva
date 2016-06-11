#ifndef _CYCLIC_QUEUE_HPP_
#define _CYCLIC_QUEUE_HPP_

// includes
#include <memory>
#include <list>
#include <queue>
#include <functional>

// forward declarations
class Unit;

/*< Cyclic queue provides simple priortiy queue in which
priority is some speed ability of units. Cyclic queue
returns to the beggining of queue if it reaches end.*/
class CyclicQueue {
public:
	CyclicQueue();
	~CyclicQueue();
	void Repush();
	/*< Returns current unit or nullptr if queue is empty. */
	std::shared_ptr<Unit> Get();
	/*< Take next step in queue. */
	bool Next();
	void Insert (std::shared_ptr<Unit> unit);
	bool IsEmpty() const;
	bool IsNewCycle() const;
	size_t GetQueueSize() const;
	size_t GetQueueBufferSize() const;
private:
	std::priority_queue<std::shared_ptr<Unit>, std::vector<std::shared_ptr<Unit>>, std::function<bool(std::shared_ptr<Unit>, std::shared_ptr<Unit>)>> queue_;
	std::list<std::shared_ptr<Unit>> to_queue_;
	bool new_cycle_;
	CyclicQueue(const CyclicQueue&) = delete;
	CyclicQueue& operaotor = (const CyclicQueue&) = delete;
};

#endif // _CYCLIC_QUEUE_HPP_
