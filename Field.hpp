#ifndef _FIELD_HPP_
#define _FIELD_HPP_

#include <memory>
#include <random>

#include "EvolvaException.hpp"

class CellObject;
class MovableObject;
class NonMovableObject;
class FieldCell;
class Unit;
class FieldPimpl;

class Field {

	friend class Tui;

private:

	std::unique_ptr<FieldPimpl> pimpl_;

	mutable size_t ffid_ = 0;


	static std::shared_ptr<Field> instance_;

	Field(const size_t x, const size_t y);
	Field() = delete;
	Field(const Field&) = delete;
	Field& operator=(const Field &) = delete;

	mutable std::default_random_engine random_generator_;
	double mutability_;
	bool InsertCellObject(std::shared_ptr<CellObject> object, const size_t x, const size_t y);

public:
	inline const size_t GetFfid() const { return ++ffid_; };
	static std::shared_ptr<Field> GetInstance(const size_t x = 0, const size_t y = 0);
	size_t GetWidth() const;
	size_t GetHeight() const;
	bool IsCorrect(const size_t x, const size_t y) const;
	std::shared_ptr<FieldCell> GetCell(const size_t x, const size_t y);
	bool InsertObject(std::shared_ptr<MovableObject> object, const size_t x, const size_t y);
	bool InsertObject(std::shared_ptr<NonMovableObject> object, const size_t x, const size_t y);
	bool MoveObject(std::shared_ptr<MovableObject> object, const long x_steps, const long y_steps);
	bool MoveObjectTo(std::shared_ptr<MovableObject> object, const size_t x, const size_t y);
	inline std::default_random_engine& Random() const { return random_generator_; };
	inline double Mutability() const { return mutability_; };
	bool Kill(Unit *unit);
	bool Kill(std::shared_ptr<Unit> unit);
	bool BeginCycle();
	void Pause();
	void Play();
	size_t GetGlobalTurnCounter() const;
	std::shared_ptr<MovableObject> GetCurrentObject();
	std::shared_ptr<Unit> GetCurrentUnit();
	std::shared_ptr<MovableObject> Next();
	std::shared_ptr<Unit> NextUnit();
	bool IsCycleEnd() const;
};

extern std::shared_ptr<Field> field;

#endif // _FIELD_HPP_


