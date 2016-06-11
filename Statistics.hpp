#ifndef _STATISTICS_HPP_
#define _STATISTICS_HPP_

// includes
#include "Field.hpp"

// forward declarations
class Statistics;
class Unit;
class Flesh;
class Tree;

template <class T> class StatisticsEntity {
friend class Statistics;
friend class Unit;
friend class Flesh;
friend class Tree;
friend class Field;
public:
	StatisticsEntity();
	StatisticsEntity(const StatisticsEntity& object);
	virtual ~StatisticsEntity();
	inline T Get() const { return value_; };
	inline double GetAvrage() const { return avrage_; };
	inline T GetMax() const { return max_; };
	inline T GetMin() const { return min_; };
	inline double GetSum() const { return sum_; };
private:
	StatisticsEntity& operator ++ (int);
	StatisticsEntity& operator -- (int);
	StatisticsEntity& operator = (const StatisticsEntity &object);
	T value_;
	double avrage_;
	T max_;
	T min_;
	double sum_;
};

template <class T> StatisticsEntity<T>::StatisticsEntity() {
	value_  = static_cast<T>(0);
	avrage_ = 0.0;
	max_ = value_;
	min_ = value_;
	sum_ = 0.0;
}

template <class T> StatisticsEntity<T>::StatisticsEntity(const StatisticsEntity<T>& object) {
	value_  = object.value_;
	avrage_ = object.avrage_;
	max_ = object.max_;
	min_ = object.min_;
	sum_ = object.sum_;
}

template <class T> StatisticsEntity<T>::~StatisticsEntity() {
}

template <class T> StatisticsEntity<T>& StatisticsEntity<T>::operator ++ (int) {
	value_ = value_ + static_cast<T>(1);
	if (value_ < min_) min_ = value_;
	if (value_ > max_) max_ = value_;
	double samples = field->GetGlobalTurnCounter() + 1;
	avrage_ = (avrage_ * (samples - 1) + value_) / samples;
	sum_ = sum_ + 1.0;
	return *this;
}

template <class T> StatisticsEntity<T>& StatisticsEntity<T>::operator -- (int) {
	value_ = value_ - static_cast<T>(1);
	if (value_ < min_) min_ = value_;
	if (value_ > max_) max_ = value_;
	double samples = field->GetGlobalTurnCounter() + 1;
	avrage_ = (avrage_ * (samples - 1) + value_) / samples;
	return *this;
}

template <class T> StatisticsEntity<T>& StatisticsEntity<T>::operator = (const StatisticsEntity<T>& object) {
	value_  = object.value_;
	avrage_ = object.avrage_;
	max_ = object.max_;
	min_ = object.min_;
	sum_ = object.sum_;
	return *this;
}

struct Statistics {
friend class StatisticsEntity<size_t>;
public:
	Statistics();
	virtual ~Statistics();
	StatisticsEntity<size_t> tree_;
	StatisticsEntity<size_t> flesh_;
	StatisticsEntity<size_t> herbivore_;
	StatisticsEntity<size_t> carnivore_;
	StatisticsEntity<size_t> omnivore_;
	StatisticsEntity<size_t> disabled_;
	StatisticsEntity<size_t> pregnant_;
	StatisticsEntity<size_t> miscarry_;
	StatisticsEntity<size_t> escape_;
	StatisticsEntity<size_t> fight_;
private:
	Statistics(const Statistics &object) = delete;
	Statistics& operator = (const Statistics&) = delete;
};

#endif // _STATISTICS_HPP_
