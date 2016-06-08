#ifndef _FIELD_HPP_
#define _FIELD_HPP_

#include <memory>
#include <random>

#include "EvolvaException.hpp"
#include "DialogProxy.hpp"

class CellObject;
class NonMovableObject;
class FieldCell;
class Unit;
class FieldPimpl;
class Statistics;

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

	mutable std::default_random_engine random_generator_; //!TODO: apply generator to all randoms
	double mutability_;
	bool InsertCellObject(std::shared_ptr<CellObject> object, const size_t x, const size_t y);
	const bool keep_grass_[9];
	const bool new_grass_[9];
	const bool keep_tree_[9];
	const bool new_tree_[9];
public:
	inline const size_t GetFfid() const { return ++ffid_; };
	static std::shared_ptr<Field> GetInstance(const size_t x = 0, const size_t y = 0);
	size_t GetWidth() const;
	size_t GetHeight() const;
	bool IsCorrect(const size_t x, const size_t y) const;
	std::shared_ptr<FieldCell> GetCell(const size_t x, const size_t y);
	bool InsertObject(std::shared_ptr<Unit> object, const size_t x, const size_t y);
	bool InsertObject(std::shared_ptr<NonMovableObject> object, const size_t x, const size_t y);
	inline bool InsertNmo(std::shared_ptr<NonMovableObject> object, const size_t x, const size_t y) { return InsertObject(object, x, y);};
	bool MoveObject(std::shared_ptr<Unit> object, const long x_steps, const long y_steps, const bool trim = false);
	bool MoveObjectTo(std::shared_ptr<Unit> object, size_t x, size_t y, const bool trim = false);
	inline std::default_random_engine& Random() const { return random_generator_; };
	inline double Mutability() const { return mutability_; };
	bool Kill(std::shared_ptr<Unit> unit, const size_t reason);
	bool KillNmo(std::shared_ptr<NonMovableObject> object);
	bool BeginCycle();
	void Pause();
	void Play();
	bool IsPauseLoop() const;
	size_t GetGlobalTurnCounter() const;
	std::shared_ptr<Unit> GetCurrentObject();
	bool Next();
	std::shared_ptr<Unit> NextUnit();
	bool IsCycleEnd() const;
	void GrowPlants();
	void MakeGrass();
	void f2();
	std::shared_ptr<Statistics> stats_;
	inline bool IsNewTurn() const { return new_turn_; };
	bool new_turn_;
};

extern std::shared_ptr<Field> field;

#endif // _FIELD_HPP_


