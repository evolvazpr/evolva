#ifndef _CELLOBJECT_H_
#define _CELLOBJECT_H_

#include <iostream>

/*
 * Abstract base class for objects.
 * bool isMovable() - needs to be implemented in inheriting classes.
 */

class CellObject {
	public:
		CellObject(){};
		virtual bool isMovable() = 0;
		virtual ~CellObject(){}
};

class Plant : public CellObject {
	private:
		const int energy_;
		const bool toxic_;

	public:
		Plant(int energy, bool toxic) : CellObject(), energy_(energy), toxic_(toxic) {}
		bool isToxic() { return toxic_; }
		int getEnergyValue() { return energy_; }
		virtual bool isMovable() { return false; }
		virtual ~Plant() {}
};

class TestUnit : public CellObject {
	public:
		TestUnit() : CellObject() {}
		virtual bool isMovable() { return true; }
};

#endif //_CELLOBJECT_H_
