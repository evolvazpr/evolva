#ifndef _FIELD_CELL_HPP_
#define _FIELD_CELL_HPP_

#include <iostream>
#include <memory>

#include "CellObject.hpp"

enum GroundType {Sand, Grass, Water};

/*
 * Field cell class
 * bool insertObject(std::shared_ptr<Object> obj):
 *		Funkcja wprowadza wskaźnik obiektu do komórki.
 *		RETURN VALUE:
 *			 false gdy komórka jest już zajmowana przez jakiś obiekt.
 *			 true gdy komórka była pusta i obiekt został wprowadzony.
 *
 *void removeObject():
 *		Funkcja usuwa wskaźnik z komórki. Komórka staje się pusta.
 *
 *GroundType getGroundType():
 *		Zwraca typ podłoża - TODO: możliwość dalszej implementacji.
 */ 

class FieldCell {
	private:
		std::weak_ptr<CellObject> object_;
		GroundType ground_type_;
  public:
		FieldCell(GroundType g_type = Sand) : ground_type_(g_type){}
		bool insertObject(std::shared_ptr<CellObject> obj);
		void removeObject();
		GroundType getGroundType();
};

#endif /* _FIELD_CELL_HPP_ */
