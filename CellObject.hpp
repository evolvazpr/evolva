#ifndef _CELL_OBJECT_H_
#define _CELL_OBJECT_H_

#include <memory>
#include "FieldCell.hpp"

// forward declarations
class Field;

class CellObject : public std::enable_shared_from_this<CellObject> {
public:
	bool types_[8];
	enum class Type : unsigned char {
		MOVABLE = 0,
		NON_MOVABLE,
		PLANT = 1,
		NON_PLANT,
		FLESH = 2,
		UNIT = 3,
		CARNIVORE = 4,
		HERBIVORE = 5,
		CREATURE = 6
	}; //enum describes type of object. Right now used only by TUI.
	//! TODO: type variable is not a good choice - we have to remove this and find another solution
	CellObject();
	CellObject(const size_t id);
	inline size_t GetId() const { return id_; }
	virtual ~CellObject() = 0;
	friend class Field;
	friend class FieldCell;
	bool GetType(const Type type) const;
	inline size_t GetX() const { return cell_.lock()->GetX(); }; // wyjebac -> a potem mozna wyjebac incl field cell
	inline size_t GetY() const { return cell_.lock()->GetY(); };
protected:
	void SetType(const Type bit, const bool value);
private:
	const size_t id_;		//Unique object's identification number
	std::weak_ptr<FieldCell> cell_;
};

class MovableObject : public CellObject {
friend class Tui;
public:
	MovableObject();
	bool Move(const int x, const int y, const bool trim = false);
	inline bool IsRemoved() const { return removed_; };
	virtual ~MovableObject();
	virtual double GetMovePriority() const;
	/*< Operator <= uses for sorting. It is used in custom class CyclingQueue.
	It should be operator < (like in other sorting algorithms), but operator <=
	provides new elements to be inserted AFTER old elements with the same comparing value. */
	inline bool operator <= (const MovableObject &object) const { return this->GetMovePriority() <= object.GetMovePriority(); };
protected:
	bool removed_;
};

class NonMovableObject : public CellObject {
public:
	NonMovableObject();
	virtual ~NonMovableObject();
};

class Plant : public NonMovableObject {
public:
	inline double GetEnergy() const { return energy_; };
	bool Eat(double energy);
	Plant();
	virtual ~Plant();
protected:
	double energy_;
};

class Tree : public Plant {
public:
	Tree(double energy);
	virtual ~Tree();
private:
	double default_energy_;
};

class NonPlant : public NonMovableObject {
public:
	NonPlant();
};

class Creature : public MovableObject {
public:
	Creature();
};

class Flesh : public NonMovableObject {
};

#endif //_CELL_OBJECT_H_
