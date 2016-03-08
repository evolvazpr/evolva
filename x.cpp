#include <string>
#include <memory>
#include <functional>
#include <random>
#include <iostream>
#include <unordered_map>

typedef std::unordered_map<std::string, double> DnaCode;

#define private public
#define protected public



class Field {
public:
	void Kill(...){ };
	std::default_random_engine generator;
	double mutability_;
};

static Field * world = new Field();


class DnaUnit {
public:
	DnaUnit();
	virtual ~DnaUnit();
protected:
	DnaUnit(std::shared_ptr<DnaCode> dna_code);
	std::shared_ptr<DnaCode> dna_code_;
	DnaCode &dna_;
	std::string dumpDna();
	std::shared_ptr<DnaCode> Mating(const std::shared_ptr<const DnaCode> dna_in) const ;
};

DnaUnit::DnaUnit() : dna_code_(std::make_shared<DnaCode>()), dna_(*dna_code_.get()) {
	dna_code_ = std::make_shared<DnaCode>();
}

DnaUnit::DnaUnit(std::shared_ptr<DnaCode> dna_code) : dna_code_(std::move(dna_code)), dna_(*dna_code_.get()) {
//	std::cout << "dna unit from code!\n\n";
}

std::shared_ptr<DnaCode> DnaUnit::Mating(const std::shared_ptr<const DnaCode> dna_in) const {
	auto dna = std::make_shared<DnaCode>();
	// crossover and mutating
	// mutation genes (m. genes affect only children, not parents)
	dna->insert(std::make_pair("mutability", 0.5 * (dna_code_->at("mutability") + dna_in->at("mutability"))));
	dna->insert(std::make_pair("mutation_stability", 0.5 * (dna_code_->at("mutation_stability") + dna_in->at("mutation_stability"))));
	// other genes
	for (auto i = dna_code_->begin(); i != dna_code_->end(); ++i) {
		// crossover
		if (i->first == "mutability" || i->first == "mutation_stability") continue;
		dna->insert(std::make_pair(i->first, 0.5 * (i->second + dna_in->at(i->first))));
		// mutating
		std::normal_distribution<double> mutation(100.0, world->mutability_ * dna->at("mutability"));
		double mutation_factor = mutation(world->generator) * 0.01;
		if (mutation_factor < 0.0) mutation_factor *= -1.0;
		if (mutation_factor >= dna->at("mutation_stability")) dna->at(i->first) = dna->at(i->first) * mutation_factor;
	}
	// returning
	return dna;
}

class DnaGenerator : public DnaUnit {
public:
	using DnaUnit::DnaUnit;
//	using DnaUnit::DnaUnit(std::shared_ptr<DnaCode> dna_code);
	using DnaUnit::dna_;
	double variability_;
	std::shared_ptr<DnaCode> Generate() const;
};

std::shared_ptr<DnaCode> DnaGenerator::Generate() const {
	std::shared_ptr<DnaCode> dna = std::make_shared<DnaCode>();
	std::normal_distribution<double> variation(100.0, variability_);
	for (auto i = dna_code_->begin(); i != dna_code_->end(); ++i) {
		double variation_factor = variation(world->generator);
		if (variation_factor < 0.0) variation_factor *= -1.0;
		dna->insert(std::make_pair(i->first, i->second * variation_factor * 0.01));
	}
	return std::move(dna);
}

class ThinkError {
public:
	ThinkError(std::function<std::string(void)> what_function);
	std::string what() const;
private:
	const std::function<std::string(void)> what_function;
};

ThinkError::ThinkError(std::function<std::string(void)> what_function) : what_function(what_function) { };

std::string ThinkError::what() const {
	return (what_function)();
}

class Corpse /*: NonPlant*/ {
};

class Unit : public DnaUnit /* : public NonPlant */{
public:
	Unit();
	Unit(std::shared_ptr<DnaCode> dna_code);
//	Unit(std::string &&config_data);
//	virtual ~Unit();
private:
	/*< Configuration for thinking and updating functions. It provides several
	parameters using to describe the behaviour of one given unit. It is inherited
	(in evolution sense) from parents in crossing and mutating processes or
	from the DNA generator. */
	/*< Status contains transitory status of a unit. It is update each turn
	using Update() method. */
		size_t turns_;										/*< Current lifetime in turns */
		bool sleep_;										/*< Is unit sleeping. */
		double fatigue_;									/*< Level of fatigue (0 - 100). */
		double health_;										/*< Level of health (0 - 100). */
		double speed_;										/*< Calculated level of speed (0 - 100). */
		double attractivity_;								/*< Calculated level of attractivity (0 - 100). */
		double strength_;									/*< Calculated level of strength. */
		double energy_;										/*< Level stored in nutritions in organism. */
		double poison_;										/*< Level of poison. */
		size_t death_;
	/*< Updates current status, checks death time. */
	void Update();
	/*< Wakes up from sleep. */
	void WakeUp();
	void CalculateDeathTime();
};

DnaUnit::~DnaUnit() {
}

Unit::Unit() : DnaUnit() {
	// Death time calculating.
//	CalculateDeathTime();
}

Unit::Unit(std::shared_ptr<DnaCode> dna_code) : DnaUnit(dna_code) {
}

void Unit::CalculateDeathTime() {
//    Unit *d = this;
	const double death_factor_u = dna_["death_time_u"] * (0.8 + 0.001 * ((dna_["agility"] + dna_["intelligence"] - dna_["speed"] - dna_["strength"])));
	const double death_factor_s = dna_["death_time_s"] - (dna_["agility"] + dna_["strength"] - dna_["intelligence"] - dna_["speed"]) / 240.0;
	std::normal_distribution<double> distribution(death_factor_u, death_factor_s);
	double death = distribution(world->generator);
	death_ = static_cast<size_t>(death - floor(death) >= 0.5 ? ceil(death) : floor(death));
}

void Unit::Update() {
	++turns_;
	if (turns_ >= death_) world->Kill(this);
	if (fatigue_ >= dna_["fatigue_death_treshold"]) world->Kill(this);
	if (poison_ > 0.0) health_ -= dna_["poison_susceptibility"] * poison_;
	if (health_ <= dna_["health_death_treshold"]) world->Kill(this);
	if (sleep_) {
		if (energy_ -= dna_["requirements.sleep_turn"] < 0.0) world->Kill(this);
		if (poison_ > 0.0) {
			poison_ -= 2.0 * dna_["poison_excertion"];
			if (poison_ < 0.0) poison_ = 0.0;
		}
		if (health_ < 100.00) {
			health_ += health_ * dna_["health_regeneration_sleep"];
			if (health_ > 100.0) health_ = 100.00;
		}
		fatigue_ -= dna_["fatigue_regeneration"];
		if (fatigue_ <= 0.0) {
			fatigue_ = 0.0;
			this->WakeUp();
		}
	}
	else {
		if (energy_ -= dna_["requirements.normal_turn"] < 0.0) world->Kill(this);
		if (poison_ > 0.0) {
			poison_ -= dna_["poison_excertion"];
			if (poison_ < 0.0) poison_ = 0.0;
		}
		if (health_ < 100.00) {
			health_ += health_ * dna_["health_regeneration"];
			if (health_ > 100.0) health_ = 100.00;
		}
	}
}

void Unit::WakeUp() {
	sleep_ = false;
};

int main() {
	Unit unit1;
	Unit unit2;
	unit1.dna_.insert(std::make_pair("attr1", 44.489));
	unit1.dna_.insert(std::make_pair("attr2", 78.004));
	unit1.dna_.insert(std::make_pair("pop", 4.7705));
	unit1.dna_["agility"] = 67.907;
	unit1.dna_["strength"] = 22.7;
	unit1.dna_["speed"]= 80.0;
	unit1.dna_["intelligence"] = 16.69;
	unit1.dna_["death_time_u"]= 70.00;
	unit1.dna_["death_time_s"]= 1.0;
	unit1.CalculateDeathTime();
	unit2.dna_.insert(std::make_pair("attr1", 21.000));
	unit2.dna_.insert(std::make_pair("attr2", 2.878));
	unit2.dna_.insert(std::make_pair("pop", 22.903));
	unit2.dna_["agility"] = 46.6;
	unit2.dna_["strength"] = 100.0;
	unit2.dna_["speed"]= 7.90;
	unit2.dna_["intelligence"] = 78.008;
	unit2.dna_["death_time_u"]= 70.00;
	unit2.dna_["death_time_s"]= 10.0;
	unit2.CalculateDeathTime();
	std::cout << "1: " << unit1.death_ << "\n\n";
	std::cout << "2: " << unit2.death_ << "\n\n";
	while (1) {
		auto dna = std::move(unit1.Mating(unit2.dna_code_));
		auto unit3 = std::make_shared<Unit>(dna);
		unit3->CalculateDeathTime();
		std::cout << "3: " << unit3->death_;
		char c = 0;
		std::cin >> c;
		std::cout << "\n";
	}
//	for (auto i = unit3->dna_.begin(); i != unit3->dna_.end(); ++i) {
 //       std::cout << i->first << " : " << i->second << "\n\n";
//	}
	return 0;
}
