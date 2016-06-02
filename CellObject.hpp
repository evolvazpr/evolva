#ifndef _CELL_OBJECT_H_
#define _CELL_OBJECT_H_

#include <memory>
#include "FieldCell.hpp"

// forward declarations
class Field;

class CellObject : public std::enable_shared_from_this<CellObject> {
public:
	enum class Type : unsigned char {
		MOVABLE = 0,
		NON_MOVABLE,
		PLANT,
		NON_PLANT,
		FLESH,
		UNIT,
		CARNIVORE,
		HERBIVORE,
		RESERVED,
		EATABLE,
		TREE
	}; //enum describes type of object. Right now used only by TUI.
	//! TODO: type variable is not a good choice - we have to remove this and find another solution
	CellObject();
	CellObject(const size_t id);
	inline size_t GetId() const { return id_; }
	virtual ~CellObject(); // was pure virtual
	friend class Field;
	friend class FieldCell;
	bool GetType(const Type type) const;
	inline size_t GetX() const { return cell_.lock()->GetX(); }; //?
	inline size_t GetY() const { return cell_.lock()->GetY(); };
protected:
	bool types_[11];
	void SetType(const Type bit, const bool value);
private:
	const size_t id_;		//Unique object's identification number
	std::weak_ptr<FieldCell> cell_;
};

class NonMovableObject : public CellObject {
public:
	NonMovableObject();
	virtual ~NonMovableObject();
};

#include "Eatable.hpp"

class Plant : public Eatable {
public:
	inline double GetEnergy() const { return energy_; };
	double Eat(double energy);
	Plant(const double energy);
	virtual ~Plant();
protected:
	double energy_;
};

class Tree : public Plant {
public:
	Tree(double energy);
	virtual ~Tree();
};

class NonPlant : public NonMovableObject {
public:
	NonPlant();
};

class Flesh : public Eatable {
public:
	Flesh(const double energy);
	virtual ~Flesh();
};

#endif //_CELL_OBJECT_H_
