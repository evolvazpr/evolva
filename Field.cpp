#include "Field.hpp"
#include "FieldCell.hpp"
#include "CellObject.hpp"
#include "CyclicQueue.hpp"
#include "Unit.hpp"
#include "Statistics.hpp"
#include <vector>
#include <boost/multi_array.hpp>

#include <iostream>
#include <random>
#include "Application.hpp"

std::shared_ptr<Field> field = nullptr;

class FieldPimpl {
friend class Field;
private:
	const size_t width_;
	const size_t height_;

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
			return "reserved";
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

Field::Field(const size_t x, const size_t y) : pimpl_(new FieldPimpl(x, y)), keep_grass_{0, 0, 0, 1, 1, 1, 0, 0, 0}, new_grass_{0, 0, 1, 0, 0, 0, 0, 0, 0}, keep_tree_{0, 0, 1, 1, 0, 0, 0, 0, 0}, new_tree_{0, 0, 1, 0, 0, 0, 0, 0, 0} {
	for (size_t i = 0; i < x; i++) {
		for (size_t j = 0; j < y; j++) {
			pimpl_->cells_[i][j] = std::make_shared<FieldCell>(i, j);
		}
	}
	stats_ = std::make_shared<Statistics>();
	new_turn_ = true;
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
	std::shared_ptr<FieldCell> cell = GetCell(x, y);
	Application *gui = Application::GetInstance();
	if (cell->IsEmpty()){
		cell->SetObject(object);
		gui->CreateObject(object, x, y);
		return true;
	}
	else {
		return false;
	}
}

bool Field::InsertObject(std::shared_ptr<Unit> object, const size_t x, const size_t y) {
	bool insertion = InsertCellObject(std::static_pointer_cast<CellObject>(object), x, y);
	if (insertion){
		pimpl_->movable_objects_.Insert(object);
		return true;
	}
	else {
		return false;
	}
}

bool Field::InsertObject(std::shared_ptr<NonMovableObject> object, const size_t x, const size_t y) {
	bool insertion = InsertCellObject(std::static_pointer_cast<CellObject>(object), x, y);
	if (insertion){
		pimpl_->non_movable_objects_.push_back(object);
		return true;
	}
	else {
		return false;
	}
}

bool Field::MoveObjectTo(std::shared_ptr<Unit> object, size_t x, size_t y, const bool trim) {
	std::shared_ptr<FieldCell> source_cell = object->cell_.lock();
	Application *gui = Application::GetInstance();
	if (source_cell->object_ != object) {
		throw EvolvaException("Serious memory problem.");
	}
	if (trim) {
		if (x >= pimpl_->width_) x = pimpl_->width_ - 1;
		if (y >= pimpl_->height_) y = pimpl_->height_ - 1;
	}
	else {
		return false;
	}
	std::shared_ptr<FieldCell> cell = GetCell(x, y);
	if (!cell->IsEmpty()) {
		return false;
	}
	source_cell->RemoveObject();
	cell->SetObject(object);
	gui->MoveObjectTo(object, x, y);
	return true;
}

bool Field::Kill(std::shared_ptr<Unit> unit, const size_t reason) {
	auto cell = GetCell(unit->GetX(), unit->GetY());
	Application *gui = Application::GetInstance();
	if (cell->GetUnit() == unit) {
		const size_t id = unit->GetId();
		*gui << "Unit "<< id << " is dead: " << pimpl_->GetReason(reason) << "\n";
		const size_t x = unit->GetX();
		const size_t y = unit->GetY();
		const double energy = unit->GetDna("normal_weight");
		const bool carnivore = unit->IsCarnivore();
		cell->RemoveObject();
		gui->RemoveObject(unit);
		unit->alive_ = false;
		unit->RemoveStatistics();
		InsertNmo(std::make_shared<Flesh>(energy, carnivore), x, y);
		*gui << "Flesh of unit " << id << " inserted at: " << x << ", " << y << "\n";
		return true;
	}
	else {
		*gui << "\nERROR: Possible lose of some object. Program will continue.\n";
		return false;
	}
}

bool Field::KillNmo(std::shared_ptr<NonMovableObject> object) {
	Application *gui = Application::GetInstance();
	for (size_t i = 0; i < pimpl_->non_movable_objects_.size(); ++i) {
		if (object ==  pimpl_->non_movable_objects_[i]) {
			auto cell = GetCell(object->GetX(), object->GetY());
			if (cell == nullptr) {
				*gui << "\nERROR: Possible lose of NonMovableObject. Program will continue.\n";
				return false;
			}
			cell->RemoveObject();
			gui->RemoveObject(object);
			if (pimpl_->non_movable_objects_.size() == 1) pimpl_->non_movable_objects_.clear();
			else if (i + 1 ==  pimpl_->non_movable_objects_.size())  pimpl_->non_movable_objects_.pop_back();
			else {
				pimpl_->non_movable_objects_[i] = pimpl_->non_movable_objects_.back();
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
	else {
		return MoveObjectTo(object, object->GetX() + x_steps, object->GetY() + y_steps, trim);
	}
}

bool Field::BeginCycle() {
	if (pimpl_->movable_objects_.IsEmpty()) {
		return false;
	}
	pimpl_->movable_objects_.Repush();
	return true;
}

std::shared_ptr<Unit> Field::GetCurrentObject() {
	return pimpl_->movable_objects_.Get();
}

bool Field::Next() {
	Application *gui = Application::GetInstance();
	// Take next turn. Then check whether the field is paused. Only non-paused
	// field pass methods such Update() and Think().
	bool next = pimpl_->movable_objects_.Next();
	if (!next || pimpl_->movable_objects_.IsEmpty()) {
		*gui << "All units are dead\n";
		return false;
	}
	if (!pimpl_->pause_) {
		new_turn_ = false;
		// If loop reaches its end, then increment global turn counter,
		// then sort (new turn, new order) and back to square one.
		if (pimpl_->movable_objects_.IsNewCycle()) {
			pimpl_->global_turns_++;
			//First, let the plants (and fleshes) grow.
			GrowPlants();
			for (size_t i = 0; i < pimpl_->non_movable_objects_.size(); ++i) {
				auto nmo = pimpl_->non_movable_objects_[i];
				if (nmo->GetType(CellObject::Type::EATABLE)) {
					std::static_pointer_cast<Eatable>(nmo)->Grow();
				}
			}
			*gui << "New turn: " << pimpl_->global_turns_ << "\n";
			new_turn_ = true;
		}
		// Then, check is object an unit, then update object (body-related
		// issues, recalculating movement priority. Then, let the unit think.
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
}

void Field::Pause() {
	pimpl_->pause_ = true;
}

size_t Field::GetGlobalTurnCounter() const {
	return pimpl_->global_turns_;
}

bool Field::IsCycleEnd() const {
	return pimpl_->movable_objects_.IsNewCycle();
}

#define ADD_NEIGHBOUR(i, j) if (IsCorrect(i, j) && GetCell(i, j)->GetGroundType() == FieldCell::Ground::GRASS) ++neighbours
#define ADD_NEIGHBOUR_TREE(i, j) if (IsCorrect(i, j) && !GetCell(i, j)->IsEmpty() && GetCell(i, j)->GetObject()->GetType(CellObject::Type::TREE)) ++neighbours

void Field::GrowPlants() {
	// Algorithm is inefficient and will fail if a new ground type is added
	FieldCell::Ground type;
	Application *gui = Application::GetInstance();
	const size_t width = GetWidth();
	const size_t height = GetHeight();
	boost::multi_array<int, 2> cells(boost::extents[width][height]);
	std::uniform_int_distribution<int> ground_prob(0, 1000);
	std::uniform_int_distribution<int> tree_prob(0, 800);
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
				*gui << "New tree (in grass) at: " << i << ", " << j << "\n";
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
			if (ground_prob(Random()) == 0) {
				GetCell(i, j)->SetGroundType(FieldCell::Ground::GRASS);
				gui->ReplaceSurfaceObject(FieldCell::Ground::GRASS, i, j);
			}
			else {
				type = cells[i][j] == 1 ? FieldCell::Ground::GRASS : FieldCell::Ground::GROUND;
				GetCell(i, j)->SetGroundType(type);
				gui->ReplaceSurfaceObject(type, i, j);
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
			if (tree_prob(Random()) == 0) {
				if (cell->IsEmpty()) {
					*gui << "New tree (random) at: " << i << ", " << j << "\n";
					InsertNmo(std::make_shared<Tree>(100.0), i, j);
				}
			}
			else {
				if (cell->IsEmpty() && cells[i][j]) {
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

void Field::MakeGrass() {
	Application *gui = Application::GetInstance();
	std::uniform_int_distribution<int> type(0, 1);
	for (size_t i = 0; i < GetWidth(); i++) {
		for (size_t j = 0; j < GetHeight(); j++) {
			if (type(field->Random()) == 0) {
				pimpl_->cells_[i][j]->SetGroundType(FieldCell::Ground::GROUND);
				gui->CreateSurfaceObject(FieldCell::Ground::GROUND, i, j);
			}
			else {
				pimpl_->cells_[i][j]->SetGroundType(FieldCell::Ground::GRASS);
				gui->CreateSurfaceObject(FieldCell::Ground::GRASS, i, j);
			}
		}
	}
}
