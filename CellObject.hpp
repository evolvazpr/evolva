#ifndef _CELLOBJECT_H_
#define _CELLOBJECT_H_

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

class Tree : public CellObject {
	public:
		Tree() : CellObject() {};
		virtual bool isMovable() { return false; };
		virtual ~Tree(){}
};
#endif //_CELLOBJECT_H_
