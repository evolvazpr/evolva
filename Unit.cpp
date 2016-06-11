#include "Field.hpp"
#include "Unit.hpp"
#include "FieldCell.hpp"
#include <boost/multi_array.hpp>
#include "Utility.hpp"
#include "Statistics.hpp"
#include "Application.hpp"

class Sense {
public:
	class Cell {
	friend class Sense;
	public:
		Cell(const size_t x, const size_t y);
		~Cell(){ };
		inline bool IsAvailable() const { return cell_ != nullptr; };
		inline int GetX() const { return x_; };
		inline int GetY() const { return y_; };
		inline std::shared_ptr<FieldCell> Get() { return cell_; };
		inline int GetEmptyX() { return empty_cell_x_; };
		inline int GetEmptyY() { return empty_cell_y_; };
	private:
		Cell() = delete;
		std::shared_ptr<FieldCell> cell_;
		int x_;
		int y_;
		inline void SetCell(std::shared_ptr<FieldCell> cell) { cell_ = cell; };
		int empty_cell_x_; // empty cell near this
		int empty_cell_y_; // empty cell near this

	};
	Sense(Unit *unit, double radius, double efficiency); // no need for shared_ptr, unit MUST exist
	~Sense();
	double radius_;
	double efficiency_;

	bool Fetch();


	template <class F> void Foreach(F function);
	bool IsRelativeEmpty(const int x, const int y) const; // works in foreach
	std::shared_ptr<Cell> GetRelativeCellConst(const int x, const int y) const;

//private:
	bool foreach_started_;
	int size_;
	Sense() = delete;
	Sense(const Sense&) = delete;
	boost::multi_array<std::shared_ptr<Cell>, 2> cells_;
	int discrete_radius_; // Also offset.
	Unit *  const unit_;
	size_t x_; // Absolute x of object.
	size_t y_; // Absolute y of object.
	int i_; // Foreach x.
	int j_; // Foreach y.
};

std::shared_ptr<Sense::Cell> Sense::GetRelativeCellConst(const int x, const int y) const {
	if (!foreach_started_) {
		return nullptr;
	}
	int requested_x = i_ + x;
	if (requested_x < 0 || requested_x > size_ - 1) {
		return nullptr;
	}
	int requested_y = j_ + y;
	if (requested_y < 0 || requested_y > size_ - 1) {
		return nullptr;
	}
	if (cells_[requested_x ][requested_y]->IsAvailable()) {
		return cells_[requested_x][requested_y];
	}
	else {
		return nullptr;
	}
}

Sense::~Sense() {
}

Sense::Cell::Cell(const size_t x, const size_t y) : x_(x), y_(y) {
	cell_ = nullptr;
}

bool Sense::IsRelativeEmpty(const int x, const int y) const {
	if (!foreach_started_) {
		return false;
	}
	auto cell = GetRelativeCellConst(x, y);
	if (cell == nullptr) return false;
	if (cell->Get()->IsEmpty()) {
		cells_[i_][j_]->empty_cell_x_ = cell->GetX();
		cells_[i_][j_]->empty_cell_y_ = cell->GetY();
		return true;
	}
	else {
		return false;
	}
}

template <class F> void Sense::Foreach(F function) {
	if (foreach_started_) {
		return;
	}
	foreach_started_ = true;
	for (j_ = 0; j_ < size_; ++j_) {
		for(i_ = 0; i_ < size_; ++i_) {
			function(cells_[i_][j_]);
		}
	}
	foreach_started_ = false;
}

Sense::Sense(Unit *unit, double radius, double efficiency) : unit_(unit) {
	radius_ = radius;
	efficiency_ = efficiency;
	foreach_started_ = false;
	discrete_radius_ = round(radius_);
	size_ = 2 * discrete_radius_ + 1;
	cells_.resize(boost::extents[size_][size_]);
	for (int i = 0; i < size_; ++i) {
		for (int j = 0; j < size_; ++j) {
			cells_[i][j] = std::make_shared<Cell>(i - discrete_radius_, j - discrete_radius_);
		}
	}
	x_ = unit->GetX();
	y_ = unit->GetY();
}

bool Sense::Fetch() {
	for (int i = -static_cast<int>(discrete_radius_); i <= static_cast<int>(discrete_radius_); ++i) {
		for (int j = -static_cast<int>(discrete_radius_); j <= static_cast<int>(discrete_radius_); ++j) {
			if (i == 0 && j == 0) {
				continue;
			}
			if (field->IsCorrect(x_ + i, y_ + j)) {
				cells_[i + discrete_radius_][j + discrete_radius_]->SetCell(field->GetCell(x_ + i, y_ + j));
			}
		}
	}
	return true;
}








Unit::Unit() : DnaUnit(), CellObject() {
	omnivore_ = false;
	disabled_ = true;
	alive_ = true;
	SetType(Type::UNIT, true);
	SetType(Type::MOVABLE, true);
	death_reason_ = 8;
	field->stats_->disabled_++;
}

Unit::Unit(std::shared_ptr<DnaCode> dna_code) : DnaUnit(dna_code), CellObject() {
	Application *gui = Application::GetInstance();
	try {
		omnivore_ = false;
		disabled_ = false;
		alive_ = true;
		SetType(Type::UNIT, true);
		turns_ = 0;
		sleep_ = false;
		fatigue_ = 0.0;
		health_ = 100.0;
		speed_ = 0.0;
		attractiveness_ = 0.0;
		strength_ = 0.0;
		energy_ = dna_["normal_weight"];
		death_ = 0;
		pregnant_ = false;
		puerperium_ = false;
		pregnant_turns_ = 0;
		child_dna_code_ = nullptr;
		CalculateDeathTime();
		UpdateSpeed();
		if (dna_["herbivore"] > 50.0) {
			SetType(Type::HERBIVORE, true);
			if (dna_["carnivore"] > 50.0) {
				SetType(Type::CARNIVORE, true);
				field->stats_->omnivore_++;
				omnivore_ = true;
			}
			else {
				field->stats_->herbivore_++;
			}
		}
		else if (dna_["carnivore"] > 50.0) {
			SetType(Type::CARNIVORE, true);
			field->stats_->carnivore_++;
		}
		else {
			field->stats_->disabled_++;
			disabled_ = true;
		}
		death_reason_ = 8;
	}
	catch (std::exception &e) {
		*gui << "DNA data is corrupted. Cannot create Unit: " << e.what() << "\n";
		throw e;
	}
}

Unit::~Unit() {
}

bool Unit::operator <= (const std::shared_ptr<Unit> object) const {
	return GetMovePriority() <= object->GetMovePriority();
}


void Unit::CalculateDeathTime() {
	const double death_factor_u = dna_["death_time_u"] * (0.8 + 0.001 * ((dna_["agility"] + dna_["intelligence"] - dna_["speed"] - dna_["strength"])));
	const double death_factor_s = dna_["death_time_s"] - (dna_["agility"] + dna_["strength"] - dna_["intelligence"] - dna_["speed"]) / 240.0;
	std::normal_distribution<double> distribution(death_factor_u, death_factor_s);
	double death = distribution(field->Random());
	death_ = static_cast<size_t>(death - floor(death) >= 0.5 ? ceil(death) : floor(death));
}

// Update()
// first subtract energy, then thunk, then add fatigue
void Unit::Update() {
	// Update the most important parameters: turns and speed.
	UpdateSpeed();
	++turns_;
	// Turn puerperium off. It can be turned on by GiveBirth() method.
	// Puerperium lasts only 1 turn (hardcoded).
	puerperium_ = false;
	// Let the unit give a a birth if it has a place to do so.
	if (pregnant_) {
		size_t pregnancy_time = static_cast<size_t>(round(dna_["pregnancy_time"]));
		if (++pregnant_turns_ >= pregnancy_time) {
			const size_t x = GetX();
			const size_t y = GetY();
			if (field->IsCorrect(x + 1, y) && field->GetCell(x + 1, y)->IsEmpty()) {
				GiveBirth(1, 0);
			}
			else if (field->IsCorrect(x, y - 1) && field->GetCell(x, y - 1)->IsEmpty()) {
				GiveBirth(0, -1);
			}
			else if (field->IsCorrect(x - 1, y) && field->GetCell(x - 1, y)->IsEmpty()) {
				GiveBirth(-1, 0);
			}
			else if (field->IsCorrect(x, y + 1) && field->GetCell(x, y + 1)->IsEmpty()) {
				GiveBirth(0, 1);
			}
		}
	}
	if (turns_ >= death_) field->Kill(GetUnit(), 0);
	if (fatigue_ >= dna_["fatigue_death_threshold"]) field->Kill(GetUnit(), 7);
	if (health_ <= dna_["health_death_threshold"]) field->Kill(GetUnit(), death_reason_);

	// attractiveness
	attractiveness_ = (dna_["speed"] + dna_["intelligence"] + dna_["strength"] + dna_["agility"]) * (0.5 + health_ / 200.0);

	// other things
	if (sleep_) {
		if ((energy_ -= dna_["requirements.sleep_turn"]) < 0.0) field->Kill(GetUnit(), death_reason_);
		if (health_ < 100.00) {
			health_ += health_ * dna_["health_regeneration_sleep"];
			if (health_ > 100.0) health_ = 100.00;
		}
		fatigue_ -= dna_["fatigue_regeneration"];
		if (fatigue_ <= dna_["wakeup_threshold"]) {
			if (fatigue_ <= 0.0) fatigue_ = 0.0;
			WakeUp();
			Think();
			fatigue_ += dna_["fatigue_turn_increase"];
		}
	}
	else {
		if ((energy_ -= dna_["requirements.normal_turn"]) < 0.0) field->Kill(GetUnit(), death_reason_);
		if (health_ < 100.00) {
			health_ += health_ * dna_["health_regeneration"];
			if (health_ > 100.0) health_ = 100.00;
		}
		if (IsAlive()){
			Think();
			fatigue_ += dna_["fatigue_turn_increase"];
			death_reason_ = 8;
		}
	}
}

void Unit::Sleep() {
	Application *gui = Application::GetInstance();
	sleep_ = true;
	*gui << "Unit " << GetId() << " fell asleep\n";
}

void Unit::WakeUp() {
	Application *gui = Application::GetInstance();
	sleep_ = false;
	*gui << "Unit " << GetId() << " woke up\n";
}

void Unit::UpdateSpeed() {
	// Calculated speed is ((NS - 100) / NW) * e + 100 .
	// Where 100 is maximum speed, NW is normal_weight (in DNA),
	// e is energy_ and NS is "speed" based on unit DNA values.
	// TODO: from point NW make it exponential droping.
	speed_ = ((0.5 * dna_["speed"] + 0.25 * dna_["agility"] + 0.125 * dna_["intelligence"] + 0.125 * dna_["strength"]) - 100.0) / dna_["normal_weight"] * energy_ + 100.0;
}

size_t Unit::Think(std::shared_ptr<Unit> attacker) {
	// Calculate steps the unit can move. C. speed is <0, 100> (can be lower),
	// so when c. speed is 100, the unit is able to move 25 steps.
	Application *gui = Application::GetInstance();
	double steps_limit = Heaviside(speed_ / 4.0);
	// Think in danger. Fight or run.
	if (!!attacker) {
		double target_speed = 0.45 * speed_ + 0.45 * dna_["agility"] + 0.1 * dna_["intelligence"];
		double attacker_speed = 0.45 * attacker->speed_ + 0.45 * attacker->dna_["agility"] + 0.1 * attacker->dna_["intelligence"];
		double target_strength = 0.1 * (0.45 * dna_["strength"] + 0.45 * dna_["agility"] + 0.1 * speed_);
		double attacker_strength = 0.1 *(0.45 * attacker->dna_["strength"] + 0.45 * attacker->dna_["agility"] + 0.1 * attacker->speed_);
		if (target_speed >= attacker_speed) { //can run
			if (target_strength < attacker_strength) {
				field->stats_->escape_++;
				return 0;
			}
		}
		// Fight: damage inflicted by opponents are calculated using gamma
		// distribution. It gives nice "real-like" damages. Here, the intelligence
		// is important. An unit can be strong and quick, but without intelligence
		// its damages would be unpredictable and weak.
		field->stats_->fight_++;
		double target_health = health_;
		double attacker_health = attacker->health_;
		std::gamma_distribution<double> target_distribution(2.0, pow(dna_["intelligence"] / 100.0, 1.2));
		std::gamma_distribution<double> attacker_distribution(2.0, pow(attacker->dna_["intelligence"] / 100.0, 1.2));
		while (target_health > 0.0 && attacker_health > 0.0) {
			const double attacker_damage = 4.0 * attacker_strength + attacker_speed;
			const double target_damage = 4.0 * target_strength + target_speed;
			attacker_health -= (target_damage * target_distribution(field->Random()));
			target_health -= (attacker_damage * attacker_distribution(field->Random()));
		}
		// Kill two opponents if both should die in this battle.
		if (target_health < 0.0) {
			field->Kill(GetUnit(), 1);
		}
		else {
			health_ = target_health;
		}
		if (attacker_health < 0.0) {
			field->Kill(attacker, 1);
		}
		else {
			attacker->health_ = attacker_health;
		}
		return 0;
	}

	// An unit has to decide: eat or mate? If mate.
	// If mate, it has to decide which unit it wants to mate with.
	// Else it has to decide what to eat (what to attack).

	// Sleep. If the unit should die because of fatigue in next turn it goes
	// to sleep. 2 * x due to fatigue calculation after Think().

	if (fatigue_ + 2 * dna_["fatigue_turn_increase"] >= dna_["fatigue_death_threshold"]) {
		Sleep();
		return 0;
	}

	// If the unit is in puerperium, it cannot go away from its child, so return.
	if (puerperium_) {
		return 2; // Puerperium code.
	}

	// Eating, mating.

	double vision_radius = dna_["vision"];

	// Hunger. The simplest approach is making hysteresis. At energy level equal
	// to normal_weight, the speed is unchanged. At 2 * normal_weight it is
	// halved. Unit does not want to be slow, but it has to eat.
	// If energy <= normal_weight, the unit is looking for food. If the unit
	// is eating, it is done, when energy level is 2 * normal_weight.
	// If the unit is not eating and energy level > normal_weight, then the
	// unit can do other things.

	if (energy_ <= dna_["normal_weight"] || pregnant_) {
		//find food, eat
		if (GetType(Type::HERBIVORE)) {
			auto best_cell_think = Action<Sense::Cell>(steps_limit,
				Heaviside((energy_ - 2 * dna_["requirements.normal_turn"] - dna_["requirements.eating"]) / dna_["requirements.step"] - 1.0),
				vision_radius,
				health_,
				([this](std::shared_ptr<Sense::Cell> cell,
						std::shared_ptr<FieldCell> field_cell,
						std::shared_ptr<CellObject> cell_object,
						bool given,
						std::shared_ptr<Sense::Cell> best_cell) -> std::shared_ptr<CellObject> {
							if (!given) {
								// If object is not plant - return nullptr;
								if (!cell_object->GetType(CellObject::Type::PLANT)) return nullptr;
								return cell_object;
							}
							else {
								double best_distance = Distance(0.0, 0.0, best_cell->GetEmptyX(), best_cell->GetEmptyY());
								double distance = Distance(0.0, 0.0, cell->GetEmptyX(), cell->GetEmptyY());
								if (distance < best_distance) {
									return cell_object;
								}
								else {
									return nullptr;
								}
							}
						}
			));
			if(best_cell_think) {
				Move(best_cell_think->GetEmptyX(), best_cell_think->GetEmptyY(), true);
				auto tree = std::dynamic_pointer_cast<Tree>(best_cell_think->Get()->GetObject());
				energy_ += tree->Eat(3.0 * dna_["normal_weight"] - energy_);
				return 0;
			}
		}
		else if (GetType(Type::CARNIVORE)) {
			auto best_cell_think = Action<Sense::Cell>(steps_limit,
				Heaviside((energy_ - 2 * dna_["requirements.normal_turn"] - dna_["requirements.eating"]) / dna_["requirements.step"] - 1.0),
				vision_radius,
				health_,
				([this](std::shared_ptr<Sense::Cell> cell,
						std::shared_ptr<FieldCell> field_cell,
						std::shared_ptr<CellObject> cell_object,
						bool given,
						std::shared_ptr<Sense::Cell> best_cell) -> std::shared_ptr<CellObject> {
							if (!given) {
								// If object is not unit or flesh - return nullptr;
								if (!cell_object->GetType(CellObject::Type::HERBIVORE) && !cell_object->GetType(CellObject::Type::FLESH)) {
									return nullptr;
								}
								return cell_object;
							}
							else {
								if (cell_object->GetType(CellObject::Type::FLESH)) {
									if (best_cell->Get()->GetObject()->GetType(CellObject::Type::UNIT)) {
										return cell_object;
									}
									else {
										return nullptr;
									}
								}
								else {
									if (cell_object->GetType(CellObject::Type::FLESH)) {
										if (!best_cell->Get()->GetObject()->GetType(CellObject::Type::FLESH)) {
											return cell_object;
										}
									}
									else if (best_cell->Get()->GetObject()->GetType(CellObject::Type::FLESH)) {
										return nullptr;
									}
									else {
										auto best_unit = std::dynamic_pointer_cast<Unit>(best_cell->Get()->GetObject());
										auto unit = std::dynamic_pointer_cast<Unit>(cell_object);
										if (best_unit == nullptr) {
											throw EvolvaException("Unit type data corrupted.");
										}
										if (unit == nullptr) {
											return nullptr;
										}
										if (best_unit->IsCarnivore() && !unit->IsCarnivore()) {
											return cell_object;
										}
									}
									double best_distance = Distance(0.0, 0.0, best_cell->GetEmptyX(), best_cell->GetEmptyY());
									double distance = Distance(0.0, 0.0, cell->GetEmptyX(), cell->GetEmptyY());
									if (distance < best_distance) {
										return cell_object;
									}
									else {
										return nullptr;
									}
								}
							}
						}
			));
			if(best_cell_think) {
				std::shared_ptr<Flesh> flesh = nullptr;
				if (best_cell_think->Get()->GetObject()->GetType(CellObject::Type::UNIT)) {
					auto unit = std::dynamic_pointer_cast<Unit>(best_cell_think->Get()->GetObject());
					Move(best_cell_think->GetEmptyX(), best_cell_think->GetEmptyY(), true);
					const size_t xc = unit->GetX();
					const size_t yc = unit->GetY();
					unit->Think(std::static_pointer_cast<Unit>(shared_from_this()));
					if(!IsAlive()) return 1;
					flesh = std::dynamic_pointer_cast<Flesh>(field->GetCell(xc, yc)->GetObject());
					if (flesh == nullptr) return 1;
					else if (!flesh->GetType(CellObject::Type::FLESH)) return 2;
				}
				else {
					flesh = std::dynamic_pointer_cast<Flesh>(best_cell_think->Get()->GetObject());
					if (flesh == nullptr) return 3;
				}
				energy_ += flesh->Eat(3.0 * dna_["normal_weight"] - energy_);
				return 0;
			}
		}
    }

    // Mating if not pregnant.
	else if (!pregnant_) {
		auto best_cell = Action<Sense::Cell>(steps_limit,
			Heaviside((energy_ - 2 * dna_["requirements.normal_turn"] - dna_["requirements.mating"]) / dna_["requirements.step"] - 1.0),
			vision_radius,
			health_,
			[this](	std::shared_ptr<Sense::Cell> cell,
					std::shared_ptr<FieldCell> field_cell,
					std::shared_ptr<CellObject> cell_object,
					bool given,
					std::shared_ptr<Sense::Cell> &best_cell) -> std::shared_ptr<CellObject> {
						if (!given) {
							if (!cell_object->GetType(CellObject::Type::UNIT)) return nullptr;
							if ((GetType(CellObject::Type::CARNIVORE) && !(cell_object->GetType(CellObject::Type::CARNIVORE))) ||
								(GetType(CellObject::Type::HERBIVORE) && !(cell_object->GetType(CellObject::Type::HERBIVORE)))) {
									return nullptr;
							}
							// If unit is pregnant take next cell.
							auto unit = std::dynamic_pointer_cast<Unit>(cell_object);
							if (unit->pregnant_) return nullptr;
							else {
								return cell_object;
							}
						}
						else {
							auto best_unit = std::dynamic_pointer_cast<Unit>(best_cell->Get()->GetObject());
							auto unit = std::dynamic_pointer_cast<Unit>(cell_object);
							if (unit->attractiveness_ > best_unit->attractiveness_) {
								return cell_object;
							}
							else if (unit->attractiveness_ == best_unit->attractiveness_) {
								if (Distance(GetX(), GetY(), unit->GetX(), unit->GetY()) < Distance(GetX(), GetY(), best_unit->GetX(), best_unit->GetY())) {
									return cell_object;
								}
								else {
									return nullptr;
								}
							}
						}
						return nullptr;
					}
		);
		if(best_cell) {
			Move(best_cell->GetEmptyX(), best_cell->GetEmptyY(), true);
			auto unit = std::dynamic_pointer_cast<Unit>(best_cell->Get()->GetObject());
			*gui << "Mating: " << GetId() << " with " << unit->GetId() << "\n";
			auto child_dna_code = Mating(unit->dna_code_);
			unit->Pregnant(child_dna_code);
			return 0;
		}
	}

	// Other cases: explore.
	return Explore(std::min(M_SQRT2 * vision_radius, steps_limit));
}


double Unit::GetMovePriority() const {
	return speed_;
}

bool Unit::Pregnant(std::shared_ptr<DnaCode> dna) {
	Application *gui = Application::GetInstance();
	if (pregnant_) return false;
	child_dna_code_ = dna;
	pregnant_ = true;
	*gui << "Unit " << GetId() << " is pregnant\n";
	return true;
}

size_t Unit::Explore(double steps) {
	std::uniform_real_distribution<double> distribution(0.0, 2.0 * M_PI);
	double angle = distribution(field->Random());
	double &magnitude = steps;
	int x = static_cast<int>(floor(magnitude * cos(angle)));
	int y = static_cast<int>(floor(magnitude * sin(angle)));
	Move(x, y, true);
	return 0;
}



bool Unit::Move(const int x, const int y, const bool trim) {
	double cost = Distance(0.0, 0.0, x, y) * dna_["requirements.step"];
	if (energy_ >= cost) {
		energy_ -= cost;
		return field->MoveObject(std::static_pointer_cast<Unit>(shared_from_this()), x, y, trim);
	}
	else {
		return false;
	}
}

template <class SC, class F> std::shared_ptr<SC> Unit::Action(double steps_limit, double max_steps, double radius, double efficiency, F conditions) {
	// Calculate steps limits.
	double steps = 0.0;
	double max_vision = M_SQRT2 * radius;
	steps = min3(max_steps, max_vision, steps_limit);
	// If there is no steps to go, return;
	if (steps < 0.0) return nullptr;
	// Get vision from object's point.
	Sense vision(this, radius, efficiency);
	vision.Fetch();
	// Create best cell.
	std::shared_ptr<Sense::Cell> best_cell;
	// Perform several checks for all available vision cells.
	vision.Foreach([this, &best_cell, &vision, steps, &conditions](std::shared_ptr<Sense::Cell> cell) -> void {
		// If cell is not available take next cell.
		if (!cell->IsAvailable()) return;
		// If object has no empty neighbors, take next cell.
		if (!vision.IsRelativeEmpty(1, 0)) {
			if(!vision.IsRelativeEmpty(-1, 0)) {
				if(!vision.IsRelativeEmpty(0, 1)) {
					if(!vision.IsRelativeEmpty(0, -1)) {
						return;
					}
				}
			}
		}
		// Create field_cell from vision cell.
		auto field_cell = cell->Get();
		// If cell is empty take next cell.
		if (field_cell->IsEmpty()) return;
		// Calculate distance. If it exceeds available steps - take next cell.
		double distance = Distance(0.0, 0.0, cell->GetX(), cell->GetY());
		if (distance >= steps) return;
		// Create cell object from field cell.
		auto cell_object = field_cell->GetObject();
		// Perform custom conditions over cell.
		auto specific_object = conditions(cell, field_cell, cell_object, false, best_cell);
		// If specific_object is nullptr - return.
		if (!specific_object) return;
		// If there is not best cell, current cell is the best.
		if (!best_cell) best_cell = cell;
		// Else, perform the second conditions test. It may change best_cell.
		else {
			auto best_object = conditions(cell, field_cell, specific_object, true, best_cell);
			if (!!best_object) {
				best_cell = cell;
			}
		}
	});
	// If there is no right object, return empty cell.
	if (!best_cell) {
		return nullptr;
	}
	// Else, perform action.
	else {
		return best_cell;
	}
}

void Unit::GiveBirth(const int x, const int y) {
	Application *gui = Application::GetInstance();
	const double child_energy = child_dna_code_->operator[]("normal_weight") * 0.75;
	if (energy_ - child_energy < 0.0) return Miscarry();
	auto child = std::make_shared<Unit>(child_dna_code_);
	field->InsertObject(child, static_cast<int>(GetX() + x), static_cast<int>(GetY()) + y);
	*gui << "Unit " << GetId() << " gives a birth to " << child->GetId() << " at " << child->GetX() << ", " << child->GetY() << " (" << GetX() + x << ", " << GetY() + y << ")\n";
	energy_ -= (dna_["requirements.childbirth"] + child_energy);
	child->energy_ = child_energy;
	pregnant_ = false;
	pregnant_turns_ = 0;
	child_dna_code_.reset();
	puerperium_ = true;
	death_reason_ = 5;
}

void Unit::Miscarry() {
	Application *gui = Application::GetInstance();
	*gui << "Unit " << GetId() << " miscarried\n";
	pregnant_ = false;
	pregnant_turns_ = 0;
	child_dna_code_.reset();
	field->stats_->miscarry_++;
}

void Unit::RemoveStatistics() {
	if (disabled_) {
		field->stats_->disabled_--;
	}
	else if (omnivore_) {
		field->stats_->omnivore_--;
	}
	else if (IsCarnivore()) {
		field->stats_->carnivore_--;
	}
	else if (IsHerbivore()) {
		field->stats_->herbivore_--;
	}
}
