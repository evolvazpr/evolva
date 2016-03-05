#include <string>
#include <memory>
#include <functional>
#include <random>
#include <iostream>

class Field {
public:
	void Kill(...){ };
};

static Field * world = new Field();





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

class Unit /* : public NonPlant */{
public:
	Unit();
//	Unit(std::string &&config_data);
//	virtual ~Unit();
private:
	/*< Configuration for thinking and updating functions. It provides several
	parameters using to describe the behaviour of one given unit. It is inherited
	(in evolution sense) from parents in crossing and mutating processes or
	from the DNA generator. */
	struct {
		double fatigue_sleep_treshold_;						/*< From this treshold a unit is sleepy. */
		double fatigue_death_treshold_;						/*< When reaches it dies. */
		double death_time_u_;								/*< "u" parameeter (normal distribution) for death time calculating. */
		double death_time_s_;								/*< "s" parameeter (normal distribution) for death time calculating. */
//		double death_time_scale; //calc
//		double death_time_const;							;
		double health_regeneration_normal_;					/*< Health regeneration coefficient during normal activity. */
		double health_regeneration_sleep_;					/*< Health regeneration coefficient during sleep. */
		double health_death_treshold_;						/*< When reaches it dies. */
		double fatigue_regeneration_;						/*< Fatigue regeneration value (during sleep). */
	} config_;
	/*< Status contains transitory status of a unit. It is update each turn
	using Update() method. */
	struct {
		size_t turns_;										/*< Current lifetime in turns */
		bool sleep_;										/*< Is unit sleeping. */
		double fatigue_;									/*< Level of fatigue (0 - 100). */
		double health_;										/*< Level of health (0 - 100). */
		double speed_;										/*< Calculated level of speed (0 - 100). */
		double attractivity_;								/*< Calculated level of attractivity (0 - 100). */
		double strength_;									/*< Calculated level of strength. */
		double energy_;										/*< Level stored in nutritions in organism. */
		double posion_;										/*< Level of poison. */
	} status_;
	struct {
		bool is_carnivore;									/*< Is unit a carnivore. */
		bool is_herbivore;									/*< Is unit a herbivore; */
		size_t death_;										/*< Precalculated time of death in turns. */
		double strength_;									/*< Level of strength (0 - 100). */
		double agility_;									/*< Level of agility (0 - 100). */
		double speed_;										/*< Level of speed (0 - 100). */
		double intelligence_;								/*< Level of intelligence (0 - 100). */
		double vision_;										/*< Level of vision performance (0 - 100). */
		double olfaction_;									/*< Level of olfaction performance (0 - 100). */
		double normal_weight_;								/*< Level of normal weight (without nutritions) (0 - 100). */
		struct {
			double normal_turn_;
			double sleep_turn_;
			double movement_per_normalized_length_;
			double mating_;
			double eating_;
			double attacking_;
			double running_;
		} requirements_;
	} attributes_;
	/*< Updates current status, checks death time. */
	void Update();
	/*< Wakes up from sleep. */
	void WakeUp();
};

Unit::Unit() {
	// Death time calculating.
	{
		const double death_factor_u = 0.8 * config_.death_time_u_ + 0.2 * (3.0 * (attributes_.agility_ + attributes_.intelligence_ - attributes_.speed_ - attributes_.strength_));
		const double death_factor_s = config_.death_time_s_ - (attributes_.agility_ + attributes_.strength_ - attributes_.intelligence_ - attributes_.speed_) / 240.0;
		std::normal_distribution<double> distribution(death_factor_u, death_factor_s);
		std::default_random_engine generator;
		double death = distribution(generator);
		attributes_.death_ = static_cast<size_t>(death - floor(death) >= 0.5 ? ceil(death) : floor(death));
	}
}

void Unit::Update() {
	++status_.turns_;
	if (status_.turns_ >= attributes_.death_) world->Kill(this);
	if (status_.fatigue_ >= config_.fatigue_death_treshold_) world->Kill(this);
	if (status_.health_ <= config_.health_death_treshold_) world->Kill(this);
	if (status_.sleep_) {
		if (status_.energy_ -= attributes_.requirements_.sleep_turn_ < 0.0) world->Kill(this);
		if (status_.health_ < 100.00) {
			status_.health_ += status_.health_ * config_.health_regeneration_sleep_;
			if (status_.health_ > 100.0) status_.health_ = 100.00;
		}
		status_.fatigue_ -= config_.fatigue_regeneration_;
		if (status_.fatigue_ <= 0.0) {
			status_.fatigue_ = 0.0;
			this->WakeUp();
		}
	}
	else {
		if (status_.energy_ -= attributes_.requirements_.normal_turn_ < 0.0) world->Kill(this);
	}
}

void Unit::WakeUp() {
	status_.sleep_ = false;
};

int main() {
	return 0;
}
