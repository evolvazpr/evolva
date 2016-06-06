#include "Field.hpp"
#include "FieldCell.hpp"
#include "CellObject.hpp"
#define private public
#include "CyclicQueue.hpp"
#undef private
#include "Unit.hpp"
#include <vector>
#include <boost/multi_array.hpp>
#include "Application.hpp"

#include <iostream>
std::shared_ptr<Field> field;
class FieldPimpl {
friend class Field;
private:
	const size_t width_; //ilosc xow
	const size_t height_; //ilosc ykow

	mutable size_t ffid_ = 0;

	boost::multi_array<std::shared_ptr<FieldCell>, 2> cells_;

	CyclicQueue movable_objects_;
	std::vector<std::shared_ptr<NonMovableObject>> non_movable_objects_;

	FieldPimpl(const size_t x, const size_t y);
	FieldPimpl() = delete;
	bool pause_;
	size_t global_turns_;

	const std::string GetReason(const size_t number) const;
public:
	~FieldPimpl();
};

const std::string FieldPimpl::GetReason(const size_t number) const {
	switch(number) {
		case 0:
			return "age";
		case 1:
			return "battle injury";
		case 2:
			return "health";
		case 3:
			return "poison";
		case 4:
			return "maternal death";
		case 5:
			return "post maternal death";
		case 6:
			return "perinatal death";
		case 7:
			return "fatigue";
		case 8:
			return "lack of energy";
		default:
			return "unknown reason";
	}
}

FieldPimpl::FieldPimpl(const size_t x, const size_t y) : width_(x), height_(y), cells_(boost::extents[x][y]) {
	pause_ = true;
	global_turns_ = 0;
}

FieldPimpl::~FieldPimpl() {
}

std::shared_ptr<Field> Field::instance_ = nullptr;

Field::Field(const size_t x, const size_t y) : pimpl_(new FieldPimpl(x, y)), keep_grass_({0, 0, 0, 1, 1, 1, 0, 0, 0}), new_grass_({0, 0, 1, 0, 0, 0, 0, 0, 0}), keep_tree_({0, 0, 1, 1, 0, 0, 0, 0, 0}), new_tree_({0, 0, 1, 0, 0, 0, 0, 0, 0}) {
	for (size_t i = 0; i < x; i++) {
		for (size_t j = 0; j < y; j++) {
			pimpl_->cells_[i][j] = std::make_shared<FieldCell>(i, j);
		}
	}
}

std::shared_ptr<Field> Field::GetInstance(const size_t x, const size_t y) {
	if (!instance_) {
		instance_ = std::shared_ptr<Field>(new Field(x, y));
		field = instance_;
	}
	return instance_;
}

size_t Field::GetWidth() const {
	return pimpl_->width_;
}

size_t Field::GetHeight() const {
	return pimpl_->height_;
}

bool Field::InsertCellObject(std::shared_ptr<CellObject> object, const size_t x, const size_t y) {
	Logic* gui = Logic::GetInstance();
	std::shared_ptr<FieldCell> cell = GetCell(x, y);
	if (cell->IsEmpty()){
		cell->SetObject(object);
		gui->CreateObject(object, x, y);
		return true;
	}
	return false;
}

bool Field::InsertObject(std::shared_ptr<Unit> object, const size_t x, const size_t y) {
	bool insertion = InsertCellObject(std::static_pointer_cast<CellObject>(object), x, y);
	if (insertion){
		pimpl_->movable_objects_.Insortion(object);
		return true;
	}
	else return false;
}

bool Field::InsertObject(std::shared_ptr<NonMovableObject> object, const size_t x, const size_t y) {
	bool insertion = InsertCellObject(std::static_pointer_cast<CellObject>(object), x, y);
	if (insertion){
		pimpl_->non_movable_objects_.push_back(object);
		NonMovableObject &grr = *(pimpl_->non_movable_objects_.back().get());
		return true;
	}
	else return false;
}

bool Field::MoveObjectTo(std::shared_ptr<Unit> object, size_t x, size_t y, const bool trim) {
	Logic* gui = Logic::GetInstance();
	std::shared_ptr<FieldCell> source_cell = object->cell_.lock();
	if (source_cell->object_ != object) {
		throw EvolvaException("Serious memory problem.");
	}
//	source_cell.reset();
//	if (!IsCorrect(x, y)){
		if (trim) {
			if (x >= pimpl_->width_) x = pimpl_->width_ - 1;
			if (y >= pimpl_->height_) y = pimpl_->height_ - 1;
		}
		else return false;
//	}
	std::shared_ptr<FieldCell> cell = GetCell(x, y);
	if (!cell->IsEmpty()) return false;
	source_cell->RemoveObject();
	cell->SetObject(object);
//	object->x_ = x;
//	object->y_ = y;
	gui->MoveObjectTo(object, x, y);
	return true;
}

bool Field::Kill(std::shared_ptr<Unit> unit, const size_t reason) {
	Logic* gui = Logic::GetInstance();
	auto cell = GetCell(unit->GetX(), unit->GetY());
	if (cell->GetUnit() == unit) {
		//logger << "Unit "<< unit->GetId() << " is dead: " << pimpl_->GetReason(reason) << "\n";
		const size_t x = unit->GetX();
		const size_t y = unit->GetY();
		const double energy = unit->GetDna("normal_weight");
		cell->RemoveObject();
		gui->RemoveObject(unit);
		unit->alive_ = false;
		std::cout << "nmobjs: " << pimpl_->non_movable_objects_.size() << "\n";
		std::cout << "\n\nINSERT FLESH!!!! :  " << energy << "  " << InsertNmo(std::make_shared<Flesh>(energy), x, y) << "\n\n";
		std::cout << "flesh: " << std::static_pointer_cast<Flesh>(GetCell(x, y)->GetObject())->GetDefaultEnergy() << "\n";
		std::cout << "nmobjs: " << pimpl_->non_movable_objects_.size() << "\n";
		return true;
	}
	else {
		//logger << "\nERROR: Possible lose of some object. Program will continue.\n";
		return false;
	}
}

bool Field::KillNmo(std::shared_ptr<NonMovableObject> object) {
	Logic* gui = Logic::GetInstance();
	for (size_t i = 0; i < pimpl_->non_movable_objects_.size(); ++i) {
		if (object ==  pimpl_->non_movable_objects_[i]) {
			auto cell = GetCell(object->GetX(), object->GetY());
			if (cell == nullptr) {
				//logger << "\nERROR: Possible lose of NonMovableObject. Program will continue.\n";
				return false;
			}
			cell->RemoveObject();
			gui->RemoveObject(object);
			if (pimpl_->non_movable_objects_.size() == 1) pimpl_->non_movable_objects_.clear();
			else if (i + 1 ==  pimpl_->non_movable_objects_.size())  pimpl_->non_movable_objects_.pop_back();
			else {
				pimpl_->non_movable_objects_[i] =  pimpl_->non_movable_objects_.back();
				pimpl_->non_movable_objects_.pop_back();
			}
			return true;
		}
	}
	return false;
}

bool Field::IsCorrect(const size_t x, const size_t y) const {
	return (x < pimpl_->width_ && y < pimpl_->height_);
}


std::shared_ptr<FieldCell> Field::GetCell(const size_t x, const size_t y) {
	if (x >= GetWidth() || y >= GetHeight()) {
		return nullptr;
	}
	return pimpl_->cells_[x][y];
}

bool Field::MoveObject(std::shared_ptr<Unit> object, const long x_steps, const long y_steps, const bool trim) {
	const bool x_trim = (x_steps < 0 && static_cast<size_t>(-x_steps) > object->GetX());
	const bool y_trim = (y_steps < 0 && static_cast<size_t>(-y_steps) > object->GetY());
	if (trim) {
		return MoveObjectTo(object, x_trim ? 0 : object->GetX() + x_steps, y_trim ? 0 : object->GetY() + y_steps, true);
	}
	else return MoveObjectTo(object, object->GetX() + x_steps, object->GetY() + y_steps, trim);
}

bool Field::BeginCycle() {
	if (pimpl_->movable_objects_.empty()) return false;
	pimpl_->movable_objects_.Begin();
	return true;
}

std::shared_ptr<Unit> Field::GetCurrentObject() {
	return pimpl_->movable_objects_.Get();
}

bool Field::Next() {
	// Take next turn. Then check whether the field is paused. Only non-paused
	// field pass methods such Update() and Think().
	pimpl_->movable_objects_.Next();
	if (pimpl_->movable_objects_.empty()) {
		std::cout << "NIOH!\n";
		return false;
	}
	if (!pimpl_->pause_) {
		// If loop reaches its end, then increment global turn counter,
		// then sort (new turn, new order) and back to square one.
		if (pimpl_->movable_objects_.IsEnd()) {
			pimpl_->global_turns_++;
			pimpl_->movable_objects_.Sort();
			pimpl_->movable_objects_.Begin();
			//First, let the plants (and fleshes) grow.
			GrowPlants();
			for (size_t i = 0; i < pimpl_->non_movable_objects_.size(); ++i) {
				auto nmo = pimpl_->non_movable_objects_[i];
				if (nmo->GetType(CellObject::Type::EATABLE)) {
					std::static_pointer_cast<Eatable>(nmo)->Grow();
				}
			}
		}
		// Then, check is object an unit, then update object (body-related
		// issues, recalculating movement priority. Then, let an unit think.
		if (pimpl_->movable_objects_.Get()->GetType(CellObject::Type::UNIT)) {
			auto unit = std::dynamic_pointer_cast<Unit>(pimpl_->movable_objects_.Get());
			unit->Update();
		}
	}
	return true;
}

std::shared_ptr<Unit> Field::NextUnit() {
	Next();
	return GetCurrentObject();
}

void Field::Play() {
	pimpl_->pause_ = false;
//	pimpl_->movable_objects_.PopEnd();
}

void Field::Pause() {
	pimpl_->pause_ = true;
//	pimpl_->movable_objects_.PushEnd();
}
/*
bool Field::IsPauseLoop() const {
	return pimpl_->movable_objects_.IsPushedEnd();
}
/**/
size_t Field::GetGlobalTurnCounter() const {
	return pimpl_->global_turns_;
}

bool Field::IsCycleEnd() const {
	return pimpl_->movable_objects_.IsEnd();
}

void Field::f1() {
	std::cout << "cqs: " << pimpl_->movable_objects_.size() << "\n";
}

void Field::f2() {
	auto z = std::dynamic_pointer_cast<Unit>(pimpl_->movable_objects_.Get());
	auto h = z;
	Pause();
	while (1) {
		std::cout << z->GetId() << "\n";
		std::cout << "preg: " << z->pregnant_ << "\n";
		std::cout << "post: " << z->puerperium_ << "\n";
		std::cout << "energy: " << z->energy_ << "\n";
		std::cout << "fat: " << z->fatigue_ << "\n\n";
		pimpl_->movable_objects_.Next();
		z = std::dynamic_pointer_cast<Unit>(pimpl_->movable_objects_.Get());
		if (z == nullptr) break;
		if (!h->IsAlive()) {
			h = z;
			z = std::dynamic_pointer_cast<Unit>(pimpl_->movable_objects_.Get());
		}
		if (z == h) break;
	}
	Play();
}

#define ADD_NEIGHBOUR(i, j) if (IsCorrect(i, j) && GetCell(i, j)->GetGroundType() == FieldCell::Ground::GRASS) ++neighbours
#define ADD_NEIGHBOUR_TREE(i, j) if (IsCorrect(i, j) && !GetCell(i, j)->IsEmpty() && GetCell(i, j)->GetObject()->GetType(CellObject::Type::TREE)) ++neighbours

void Field::GrowPlants() {
	Logic *gui = Logic::GetInstance();
	// Algorithm is inefficient and will fail if a new ground type is added
	const size_t width = GetWidth();
	const size_t height = GetHeight();
	boost::multi_array<int, 2> cells(boost::extents[width][height]);
	for (size_t i = 0; i < width; ++i) {
		for (size_t j = 0; j < height; ++j) {
			auto cell = GetCell(i, j);
			int neighbours = 0;
			ADD_NEIGHBOUR(i - 1, j - 1);
			ADD_NEIGHBOUR(i - 1, j);
			ADD_NEIGHBOUR(i - 1, j + 1);
			ADD_NEIGHBOUR(i, j - 1);
			ADD_NEIGHBOUR(i, j + 1);
			ADD_NEIGHBOUR(i + 1, j - 1);
			ADD_NEIGHBOUR(i + 1, j);
			ADD_NEIGHBOUR(i + 1, j + 1);
			if (cell->GetGroundType() == FieldCell::Ground::GRASS && neighbours == 8 && cell->IsEmpty()) {
				std::cout << "New tree (in grass) at: " << i << ", " << j << "\n";
				InsertNmo(std::make_shared<Tree>(100.0), i, j);
			}
			if (cell->GetGroundType() == FieldCell::Ground::GROUND && new_grass_[neighbours]){
				cells[i][j] = 1;
			}
			else if (cell->GetGroundType() == FieldCell::Ground::GRASS && keep_grass_[neighbours]) {
				cells[i][j] = 1;
			}
			else {
				cells[i][j] = 0;
			}
		}
	}
	for (size_t i = 0; i < width; ++i) {
		for (size_t j = 0; j < height; ++j) {
			if (rand() % 5 == 0) {
				GetCell(i, j)->SetGroundType(FieldCell::Ground::GRASS);
				gui->RemoveSurfaceObject(i, j);
				gui->CreateSurfaceObject(FieldCell::Ground::GRASS, i, j);
			}
			else {
				GetCell(i, j)->SetGroundType(cells[i][j] == 1 ? FieldCell::Ground::GRASS : FieldCell::Ground::GROUND);
				gui->RemoveSurfaceObject(i, j);
				gui->CreateSurfaceObject(cells[i][j] == 1 ? FieldCell::Ground::GRASS : FieldCell::Ground::GROUND, i, j);
			}
			cells[i][j] = 0;
		}
	}
	for (size_t i = 0; i < width; ++i) {
		for (size_t j = 0; j < height; ++j) {
            auto cell = GetCell(i, j);
			int neighbours = 0;
			ADD_NEIGHBOUR_TREE(i - 1, j - 1);
			ADD_NEIGHBOUR_TREE(i - 1, j);
			ADD_NEIGHBOUR_TREE(i - 1, j + 1);
			ADD_NEIGHBOUR_TREE(i, j - 1);
			ADD_NEIGHBOUR_TREE(i, j + 1);
			ADD_NEIGHBOUR_TREE(i + 1, j - 1);
			ADD_NEIGHBOUR_TREE(i + 1, j);
			ADD_NEIGHBOUR_TREE(i + 1, j + 1);
			if (cell->IsEmpty() && new_tree_[neighbours]) {
				cells[i][j] = 1;
			}
			else if (!cell->IsEmpty() && cell->GetObject()->GetType(CellObject::Type::TREE) && keep_tree_[neighbours]) {
				cells[i][j] = 1;
			}
			else {
				cells[i][j] = 0;
			}
		}
	}
	for (size_t i = 0; i < width; ++i) {
		for (size_t j = 0; j < height; ++j) {
			auto cell = GetCell(i, j);
			if (rand() % 800 == 0) {
				if (cell->IsEmpty()) {
					std::cout << "New tree (random) at: " << i << ", " << j << "\n";
					InsertNmo(std::make_shared<Tree>(100.0), i, j);
				}
			}
			else {
				if (cell->IsEmpty() && cells[i][j]) {
				//	std::cout << "NEW TREEE!\n";
					InsertNmo(std::make_shared<Tree>(100.0), i, j);
				}
				else if (!cell->IsEmpty()) {
					if (cell->GetObject()->GetType(CellObject::Type::TREE) && !cells[i][j]) {
						KillNmo(cell->GetNmo());
					}
				}
			}
		}
	}
}

void Field::LogAllUnits() const {
	return;
}
