#include "Field.hpp"
#include "Unit.hpp"
#include "FieldCell.hpp"
#include <forward_list>
#include <boost/multi_array.hpp>
#include <cmath>







// TODO: Unnatural breeding. Children may have more energy than parent! It brakes
// the first law of thermodynamics. :( <- FIX THAT!

double Heaviside(const double x) { return (x <= 0.0 ? 0.0 : x); }
size_t Abs(const int x) { return (x < 0 ? -x : x); };













class Sense {
public:
	class Cell {
	friend class Sense;
	public:
		Cell(const size_t x, const size_t y);
		~Cell(){ };
		inline bool IsAvailable() const { return cell_ != nullptr; };
		inline int GetX() const { return x_; }; //gdzie jest w sensie
		inline int GetY() const { return y_; };
		inline std::shared_ptr</*const*/FieldCell> Get()/* const*/ { return cell_; };
		inline std::shared_ptr<FieldCell> GetEmpty() { return empty_cell_; };
	private:
		Cell() = delete;
		std::shared_ptr<FieldCell> cell_;
		int x_; //gdzie jest w sensie
		int y_;
		inline void SetCell(std::shared_ptr<FieldCell> cell) { cell_ = cell; };
		std::shared_ptr<FieldCell> empty_cell_;

	};
	Sense(Unit *unit, double radius, double efficiency);
	~Sense() { };
	double radius_;
	double efficiency_;

	bool IsRelativeEmpty(const int x, const int y) const; // works in foreach
	std::pair<int, int> GetFirstEmptyNeighbor(const int x, const int y) const;
	bool Fetch();


	template <class F> void Foreach(F function);

//private:
	size_t size_;
	Sense() = delete;
	Sense(const Sense&) = delete;
	boost::multi_array<std::shared_ptr<Cell>, 2> cells_;
	size_t discrete_radius_;
	Unit *  const unit_;
	size_t x_; //absolute
	size_t y_; //absolute
	size_t i_; // foreach
	size_t j_; // foreach
};

Sense::Cell::Cell(const size_t x, const size_t y) : x_(x), y_(y) {
	cell_ = nullptr;
	empty_cell_ = nullptr;
};

bool Sense::IsRelativeEmpty(const int x, const int y) const {
	int x0 = cells_[i_][j_]->GetX();
	int y0 = cells_[i_][j_]->GetY();
	if (x < 0 && -x > x0) return false;
	if (y < 0 && -y > y0) return false;
	if (x > 0 && static_cast<size_t>(x) >= size_ - x0 - 1) return false;
	if (y > 0 && static_cast<size_t>(y) >= size_ - y0 - 1) return false;
	if (!cells_[i_ + x][j_ + y]->IsAvailable()) return false;
	if (cells_[i_ + x][j_ + y]->Get()->IsEmpty()) {
		cells_[i_][j_]->empty_cell_ = cells_[i_ + x][j_ + y]->Get();
		return true;
	}
	else return false;
}

std::pair<int, int> Sense::GetFirstEmptyNeighbor(const int x, const int y) const {
	std::shared_ptr<Field> field = Field::GetInstance();
	auto east = field->GetCell(x_ + x + 1, y_ + y);
	if (east && east->IsEmpty()) return std::make_pair(1, 0);
	auto north = field->GetCell(x_ + x, y_ + y - 1);
	if (north && east->IsEmpty()) return std::make_pair(0, -1);
	auto west = field->GetCell(x_ + x - 1, y_ + y);
	if (west && east->IsEmpty()) return std::make_pair(-1, 0);
	auto south = field->GetCell(x_ + x, y_ + y + 1);
	if (south && east->IsEmpty()) return std::make_pair(0, 1);
	throw; // :(
}

template <class F> void Sense::Foreach(F function) {

	for(i_ = 0; i_ < size_; ++i_) {
		for (j_ = 0; j_ < size_; ++j_) {
	//		auto cccx = cells_[i_][j_];
	//		auto gfg = cccx.get();
			function(cells_[i_][j_]);
		}
	}
}

Sense::Sense(Unit *unit, double radius, double efficiency) : unit_(unit) {
	radius_ = radius;
	efficiency_ = efficiency;
	discrete_radius_ = round(radius_);
	cells_.resize(boost::extents[2 * discrete_radius_ + 1][2 * discrete_radius_ + 1]);
	size_ = 2 * discrete_radius_ + 1;
	for (size_t i = 0; i < size_; ++i) {
		for (size_t j = 0; j < size_; ++j) {
			cells_[i][j] = std::make_shared<Cell>(i, j);
		}
	}
	x_ = unit->GetX();
	y_ = unit->GetY();
};

bool Sense::Fetch() {
	std::shared_ptr<Field> field = Field::GetInstance();
	int a = -discrete_radius_ + 1;
	int i = a;
	int b = discrete_radius_ - 1;
	for (; i < b; ++i) {
		int c = -discrete_radius_ + 1;
		int j = c;
		int d = discrete_radius_ - 1;
		for (; j < d; ++j) {
			if (field->IsCorrect(x_ + i, y_ + j)) {
				cells_[i + discrete_radius_][j + discrete_radius_]->SetCell(field->GetCell(x_ + i, y_ + j));
			}
		}
	}
	return true;
}








Unit::Unit() : DnaUnit(), Creature() {
	SetType(Type::UNIT, true);
}

Unit::Unit(std::shared_ptr<DnaCode> dna_code) : DnaUnit(dna_code), Creature() {
	SetType(Type::UNIT, true);
	CalculateDeathTime();
	energy_ = dna_["normal_weight"];
	UpdateSpeed();
	if (dna_["herbivore"] > 50.0) SetType(Type::HERBIVORE, true);
	if (dna_["carnivore"] > 50.0) SetType(Type::CARNIVORE, true);
	health_ = 100.0;
}

Unit::~Unit() {
}

void Unit::CalculateDeathTime() {
	std::shared_ptr<Field> field = Field::GetInstance();
	const double death_factor_u = dna_["death_time_u"] * (0.8 + 0.001 * ((dna_["agility"] + dna_["intelligence"] - dna_["speed"] - dna_["strength"])));
	const double death_factor_s = dna_["death_time_s"] - (dna_["agility"] + dna_["strength"] - dna_["intelligence"] - dna_["speed"]) / 240.0;
	std::normal_distribution<double> distribution(death_factor_u, death_factor_s);
	double death = distribution(field->Random());
	death_ = static_cast<size_t>(death - floor(death) >= 0.5 ? ceil(death) : floor(death));
}

// najpierw usuwamy energię, potem thunk, potem dodajemy zmeczenie

void Unit::Update() {
	std::shared_ptr<Field> field = Field::GetInstance();
	// Update the most important parameters: turns and speed.
	UpdateSpeed();
	++turns_;
	// Let the unit give a a birth if it has a place to do so.
	if (pregnant_) {
		if (++pregnant_turns_ >= static_cast<size_t>(dna_["pregnancy_time"])) {
			// TODO: is free space (von Neumann neigh.), then give a birth (delete return)
			return;
		}
	}
	if (turns_ >= death_) field->Kill(this);
	if (fatigue_ >= dna_["fatigue_death_threshold"]) field->Kill(this);
	if (poison_ > 0.0) health_ -= dna_["poison_susceptibility"] * poison_;
	if (health_ <= dna_["health_death_threshold"]) field->Kill(this);

	// attractiveness
	attractiveness_ = (dna_["speed"] + dna_["intelligence"] + dna_["strength"] + dna_["agility"]) * (0.5 + health_ / 200.0);

	if (sleep_) {
		if (energy_ -= dna_["requirements.sleep_turn"] < 0.0) field->Kill(this);
		if (poison_ > 0.0) {
			poison_ -= 2.0 * dna_["poison_excertion"];
			if (poison_ < 0.0) poison_ = 0.0;
		}
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
		if (energy_ -= dna_["requirements.normal_turn"] < 0.0) field->Kill(this);
		if (poison_ > 0.0) {
			poison_ -= dna_["poison_excertion"];
			if (poison_ < 0.0) poison_ = 0.0;
		}
		if (health_ < 100.00) {
			health_ += health_ * dna_["health_regeneration"];
			if (health_ > 100.0) health_ = 100.00;
		}
		Think();
		fatigue_ += dna_["fatigue_turn_increase"];
	}
}

void Unit::Sleep() {
	sleep_ = true;
}

void Unit::WakeUp() {
	sleep_ = false;
};

void Unit::UpdateSpeed() {
	// Calculated speed is ((NS - 100) / NW) * e + 100 .
	// Where 100 is maximum speed, NW is normal_weight (in DNA),
	// e is energy_ and NS is "speed" based on unit DNA values.
	// TODO: from point NW make it exponential droping.
	speed_ = ((0.5 * dna_["speed"] + 0.25 * dna_["agility"] + 0.125 * dna_["intelligence"] + 0.125 * dna_["strength"]) - 100.0) / dna_["normal_weight"] * energy_ + 100.0;
}






double Distance(double x1, double y1, double x2, double y2) {
	x1 -= x2;
	y1 -= y2;
	return sqrt(x1 * x1 + y1 * y1);
}

enum class Direction : unsigned char {
	NONE = 0,
	TOP,
	BOTTOM,
	LEFT,
	RIGHT
};


#include <iostream>



size_t Unit::Think(const double intelligence, std::shared_ptr<Unit> attacker) {
	std::shared_ptr<Field> field = Field::GetInstance();
	// Calculate steps the unit can move. C. speed is <0, 100> (can be lower),
	// so when c. speed is 100, the unit is able to move 25 steps.
	double steps = Heaviside(speed_ / 4.0);
	// If intelligence is specified, method uses "swarm intelligence".
	const double pushed_intelligence = dna_["intelligence"];
	if (!std::isnan(intelligence)) dna_["intelligence"] = intelligence;
	// For ergonomy.
	DnaCode &dna = dna_;
	// Think in danger. Fight or run.
	// TODO: is UpdateSpeed() necessary here?
	if (!!attacker) {
		double target_speed = 0.45 * speed_ + 0.45 * dna["agility"] + 0.1 * dna["intelligence"];
		double attacker_speed = 0.45 * attacker->speed_ + 0.45 * attacker->dna_["agility"] + 0.1 * attacker->dna_["intelligence"];
		double target_strength = 0.1 * (0.45 * dna["strength"] + 0.45 * dna["agility"] + 0.1 * speed_);
		double attacker_strength = 0.1 *(0.45 * attacker->dna_["strength"] + 0.45 * attacker->dna_["agility"] + 0.1 * attacker->speed_);
		if (target_speed >= attacker_speed) { //can run
			if (target_strength < attacker_strength) {
				// TODO: run
				// Run and return
				if (!std::isnan(intelligence)) dna_["intelligence"] = pushed_intelligence;
				return 0;
			}
		}
		// Fight: damage inflicted by opponents are calculated using gamma
		// distribution. It gives nice "real-like" damages. Here, the intelligence
		// is important. An unit can be strong and quick, but without intelligence
		// its damages would be unpredictable and weak.
		double target_health = health_;
		double attacker_health = attacker->health_;
		std::gamma_distribution<double> target_distribution(2.0, pow(dna["intelligence"] / 100.0, 1.2));
		std::gamma_distribution<double> attacker_distribution(2.0, pow(attacker->dna_["intelligence"] / 100.0, 1.2));
		while (target_health > 0.0 || attacker_health > 0.0) {
			attacker_health -= target_distribution(field->Random());
			target_health -= attacker_distribution(field->Random());
		}
		// Kill two opponents if both should die in this battle.
		if (target_health < 0.0) field->Kill(this);
		else health_ = target_health;
		if (attacker_health < 0.0) field->Kill(attacker);
		else attacker->health_ = attacker_health;
		// Back to original intelligence, and then return.
		if (!std::isnan(intelligence)) dna_["intelligence"] = pushed_intelligence;
		return 0;
	}

	// An unit has to decide: eat or mate? If mate.
	// If mate, it has to decide which unit it wants to mate with.
	// Else it has to decide what to eat (what to attack).

	// The most important is to give a birth to a child.
	if (pregnant_ && pregnant_turns_ >= static_cast<size_t>(dna_["pregnancy_time"])) {
		// TODO: free space, go to free space, give a birth
	}

	// Sleep. If the unit should die because of fatigue in next turn it goes
	// to sleep. 2 * x due to fatigue calculation after Think().

	if (fatigue_ + 2 * dna_["fatigue_turn_increase"] >= dna_["fatigue_death_threshold"]) {
		Sleep();
		return 0;
	}

	// Hunger. The simplest approach is making hysteresis. At energy level equal
	// to normal_weight, the speed is unchanged. At 2 * normal_weight it is
	// halved. Unit does not want to be slow, but it has to eat.
	// If energy <= normal_weight, the unit is looking for food. If the unit
	// is eating, it is done, when energy level is 2 * normal_weight.
	// If the unit is not eating and energy level > normal_weight, then the
	// unit can do other things.

    if (energy_ <= dna_["normal_weight"]) {
		//find food, eat
		if (GetType(Type::HERBIVORE)) {





			double max_steps = (energy_ - 2 * dna["requirements.normal_turn"] - dna["requirements.eating"]) / dna["requirements.step"] - 1.0;
			if (max_steps < steps) steps = max_steps;
			Sense vision(this, 2.0, 100.0);

			vision.Fetch();

			vision.Foreach([&vision](std::shared_ptr<Sense::Cell> cell) -> void {
				std::cout << cell->IsAvailable();
				if (vision.j_ + 1 == vision.size_) std::cout << "\n";
			});
			std::cout << "\n\n";

			std::shared_ptr<Sense::Cell> best_cell;
			vision.Foreach([this, &best_cell, &vision, steps](std::shared_ptr<Sense::Cell> cell) -> void {
			// If cell is not available take next cell.
			if (!cell->IsAvailable()) return;
			// If object has no empty neighbors, take next cell.
			if (!(	vision.IsRelativeEmpty(1, 0) ||
					vision.IsRelativeEmpty(-1, 0) ||
					vision.IsRelativeEmpty(0, 1) ||
					vision.IsRelativeEmpty(0, -1) )) {
						return;
			}
			// If cell is empty take next cell.
			auto field_cell = cell->Get();
			if (field_cell->IsEmpty()) return;
			// If object type is not tree
			// take next cell.
			auto cell_object = field_cell->GetObject();
			if (!cell_object->GetType(CellObject::Type::PLANT)) return;
			// If unit is pregnant take next cell. plant xd
			auto tree = std::dynamic_pointer_cast<Tree>(cell_object);
			// Calculate distance. If it exceed available steps - take next cell.
			double distance = Distance(0.0, 0.0, cell->GetX(), cell->GetY());
			if (distance >= steps) return;
			// If best cell is none, set best cell to current cell. (First Cell)
			if (!best_cell) {
				best_cell = cell;
			}
			// Else: if new unit attractiveness is > than the best units' - set new best unit.
			// If they're the same: set best unit closer to the unit.
			else {
				auto best_unit = std::dynamic_pointer_cast<Tree>(best_cell->Get()->GetObject());
				if (Distance(GetX(), GetY(), tree->GetX(), tree->GetY()) < Distance(GetX(), GetY(), best_unit->GetX(), best_unit->GetY())) {
					best_cell = cell;
				}
			}
		});
		// If there is no right object, move random direction and check again.
		if (!best_cell) {
			return Explore(steps);
		}
		else {
			Move(best_cell->GetEmpty()->GetX(), best_cell->GetEmpty()->GetX(), true);
			auto tree = std::dynamic_pointer_cast<Tree>(best_cell->Get()->GetObject());
			tree->Eat(tree->GetEnergy());
			return 0; //?
		}
	}







		return 0;
    }

    // Mating if not pregnant.
	else if (!pregnant_) {
		// Calculate maximum steps based on energy and requirements.
		double max_steps = (energy_ - 2 * dna["requirements.normal_turn"] - dna["requirements.mating"]) / dna["requirements.step"] - 1.0;
		if (max_steps < steps) steps = max_steps;
		// Let the unit go until it has enough energy. Stop when mating is done.
		// vision is variable representing vision sense. direction represents random
		// direction where the unit should go if there is no one to mate with.
		Sense vision(this, 2.0, 100.0);
		// mating
		// get fuckab;e units from vision (sense) foreach, go to the most fuckable
		// if no fuckable go to the direction (uns. char.)
		// count can you go to your crash! its better to fuck than to to stay next to your crush and dont fuck
		std::shared_ptr<Sense::Cell> best_cell;
		vision.Foreach([this, &best_cell, &vision, steps](std::shared_ptr<Sense::Cell> cell) -> void {
			// If cell is not available take next cell.
			if (!cell->IsAvailable()) return;
			// If object has no empty neighbors, take next cell.
			if (!(	vision.IsRelativeEmpty(1, 0) ||
					vision.IsRelativeEmpty(-1, 0) ||
					vision.IsRelativeEmpty(0, 1) ||
					vision.IsRelativeEmpty(0, -1) )) {
						return;
			}
			// If cell is empty take next cell.
			auto field_cell = cell->Get();
			if (field_cell->IsEmpty()) return;
			// If object type is not UNIT and the same (carnivore / herbivore)
			// take next cell.
			auto cell_object = field_cell->GetObject();
			if (!cell_object->GetType(CellObject::Type::UNIT)) return;
			if (!(GetType(CellObject::Type::CARNIVORE) && cell_object->GetType(CellObject::Type::CARNIVORE) ||
				GetType(CellObject::Type::HERBIVORE) && cell_object->GetType(CellObject::Type::HERBIVORE))) {
					return;
			}
			// If unit is pregnant take next cell.
			auto unit = std::dynamic_pointer_cast<Unit>(cell_object);
			if (unit->pregnant_) return;
			// Calculate distance. If it exceed available steps - take next cell.
			double distance = Distance(0.0, 0.0, cell->GetX(), cell->GetY());
			if (distance >= steps) return;
			// If best cell is none, set best cell to current cell. (First Cell)
			if (!best_cell) {
				best_cell = cell;
			}
			// Else: if new unit attractiveness is > than the best units' - set new best unit.
			// If they're the same: set best unit closer to the unit.
			else {
				auto best_unit = std::dynamic_pointer_cast<Unit>(best_cell->Get()->GetObject());
				if (unit->attractiveness_ > best_unit->attractiveness_) best_cell = cell;
				else if (unit->attractiveness_ == best_unit->attractiveness_) {
					if (Distance(GetX(), GetY(), unit->GetX(), unit->GetY()) < Distance(GetX(), GetY(), best_unit->GetX(), best_unit->GetY())) {
						best_cell = cell;
					}
				}
			}
		});
		// If there is no right object, move random direction and check again.
		if (!best_cell) {
			return Explore(steps);
		}
		else {
			Move(best_cell->GetEmpty()->GetX(), best_cell->GetEmpty()->GetX(), true);
			auto unit = std::dynamic_pointer_cast<Unit>(best_cell->Get()->GetObject());
			auto child_dna_code = Mating(unit->dna_code_);
			unit->Pregnant(child_dna_code);
			return 0; //?
		}
	}
	// Not pregnant -> explore.
	else {
		return Explore(steps);
	}
}


double Unit::GetMovePriority() const {
	return speed_;
}

bool Unit::Pregnant(std::shared_ptr<DnaCode> dna) {
	if (pregnant_) return false;
	child_dna_code_ = dna;
	pregnant_ = true;
	return true;
}

size_t Unit::Explore(double steps) {
	std::shared_ptr<Field> field = Field::GetInstance();
	std::uniform_real_distribution<double> distribution(0.0, 2.0 * M_PI);
	double angle = distribution(field->Random());
	double &magnitude = steps;
	int x = static_cast<int>(floor(magnitude * cos(angle)));
	int y = static_cast<int>(floor(magnitude * sin(angle)));
/*	bool movement = */Move(x, y, true);
	return 0;
	// TODO: gui -> log -> succcesful movement
}


/*

1. Po matingu jest sie w ciazy - dlugosc ciazy jest w dna i zeby urodzic diecko TRZEBA mieć wolna
przestrzen w sasiedztwei von numanna. Jesli sie nie ma - dziecko nie prezyje porodu. Kazda matka
W PIERWSZEJ KOLEJNOSCI bedzia chciala zapewnic potomstwu narodziny. Przed swoim glodem, zmeczeniem
itd.

2. Vision powinno wygladac tak, ze:
	jaskas funckja zwraca mapę (niestety chyba boost multi array :( ), ktora jest wycinkiem calego
	fieldu. Ten wycinek w srodku ma dana postac i oczywiscie jest kwadratowy. Kazdy element bedzie
	mial 2 wlasciwosci: czy w ogole jest dostepny i jaka mafizycznie komorke.
	Jesli nie jest dostepny to nie jest. Jesli jest to ma jakas komorke. Potrzebne jest to do tego
	zeby mozna bylo zrobic okreagle pola widocznosci.

	Te dane (tzn. wycieta mape) sa przekazywane do metody think. Jednostka robi ruch i znowu idzie
	vision. I tak caly czas. Algorytmy beda bardzo heurustyczne: jedzenie najblizsze, nietrujace
	z wystarczajaca iloscia energii.

	Rozmnazanie: najatrakcyjenijszy osobik jak nablizej.

	Atak / jedzenie: jak najslabszy osobnik, jak najwiekszy jak najblizej.

	Zdolnosc pojmowania swiata jest determinowana przez inteligencje. Dlatego inteligencja roju
	ma duze znaczenie.


	3. Przeniesc SenseFieldCell do FieldCell.

	4. Zrobic Mid... Circle... w Field: robienie wycinkow. <- tam juz jest ten jebany boost ma.

	5. WYliczanie jednostek vision :(

	6. Wplywa zdrowia i zmecznia na wsyztskie wlasciwosci :)

*/

