#ifndef _FIELD_CELL_HPP_
#define _FIELD_CELL_HPP_

#include <iostream>
#include <memory>

enum GroundType {Sand, Grass, Water};

/*
 * Abstract base class for objects.
 * bool isMovable() - needs to be implemented in inheriting classes.
 */

class Object {
	public:
		Object(){};
		virtual bool isMovable() = 0;
		virtual ~Object(){}
};

class Tree : public Object {
	public:
		Tree() : Object() {};
		virtual bool isMovable() { return false; };
		virtual ~Tree(){}
};

/*
 * Field cell class
 * bool insertObject(std::shared_ptr<Object> obj):
 *		Funkcja wprowadza wskaźnik obiektu do komórki.
 *		RETURN VALUE:
 *			 true gdy komórka jest już zajmowana przez jakiś obiekt.
 *			 false gdy komórka jest pusta.
 *
 *void removeObject():
 *		Funkcja usuwa wskaźnik z komórki. Komórka staje się pusta.
 *
 *GroundType getGroundType():
 *		Zwraca typ podłoża - TODO: możliwość dalszej implementacji.
 */ 

class FieldCell {
	private:
		std::weak_ptr<Object> object_;
		GroundType ground_type_;
  public:
		FieldCell(GroundType g_type = Sand) : ground_type_(g_type){}
		bool insertObject(std::shared_ptr<Object> obj);
		void removeObject();
		GroundType getGroundType();
		~FieldCell() {}
};

#endif /* _FIELD_CELL_HPP_ */
