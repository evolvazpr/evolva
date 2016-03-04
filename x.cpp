#include <string>
#include <unordered_map>
#include <memory>
#include <functional>
#include <random>
#include <iostream>

class World {
public:
	void death(...){ };
};

static World * world = new World();

struct Requirements{
	double normal_turn_;
	double sleep_turn_;
	double movement_per_normalized_length_;
	double mating_;
	double eating_;
	double attacking_;
	double running_;
};

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


class Unit {
public:
	Unit();
//	Unit(std::string &&config_data);
//	virtual ~Unit();
private:
	size_t turns_;
	size_t death_;
	struct {
		double fatigue_sleep_treshold_;
		double fatigue_death_treshold_;
		double death_time_u_;
		double death_time_s_;
//		double death_time_scale; //calc
		double death_time_const;
		double health_regeneration_normal_;
		double health_regeneration_sleep_;
		double health_death_treshold_;
		double fatigue_regeneration_;
	} config_;
	struct {
		bool sleep_;
		double fatigue_;
		double health_;
		double speed_;
		double attractivity_;
		double strength_;
	} status_;
	struct {
		double strength_;
		double agility_;
		double speed_;
		double intelligence_;
		double vision_;
		double olfaction_;
		double normal_weight_;
	} attributes_;
	std::unordered_map<size_t, std::unique_ptr<Requirements>> requirements_;
	void Update();
//	ThinkError Think();
//	std::string dumpDNA() const;
	void WakeUp();
};

Unit::Unit() {
	//wyliczanie innych rzeczy...
	{
		const double death_factor_u = 0.8 * config_.death_time_u_ + 0.2 * (3.0 * (attributes_.agility_ + attributes_.intelligence_ - attributes_.speed_ - attributes_.strength_));
		const double death_factor_s = config_.death_time_s_ - (attributes_.agility_ + attributes_.strength_ - attributes_.intelligence_ - attributes_.speed_) / 240.0;
		std::normal_distribution<double> distribution(death_factor_u, death_factor_s);
		std::default_random_engine generator;
		double death = distribution(generator);
		death_ = static_cast<size_t>(death - floor(death) >= 0.5 ? ceil(death) : floor(death));
	}
}

void Unit::Update() {
	++turns_;
	if (turns_ >= death_) world->death(this);
	if (status_.fatigue_ >= config_.fatigue_death_treshold_) world->death(this);
	if (status_.health_ <= config_.health_death_treshold_) world->death(this);
	if (status_.sleep_) {
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
		std::cout << "jidjid";
	}
}

void Unit::WakeUp() { };

int main() {
	return 0;
}
