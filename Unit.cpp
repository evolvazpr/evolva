#include "Unit.hpp"


Unit::Unit() : DnaUnit() {
	//std::cout << "UNIT!\n";
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
		if (fatigue_ <= dna_["wakeup_treshold"]) Think();
		else if (fatigue_ <= 0.0) {
			fatigue_ = 0.0;
			WakeUp();
			Think();
			fatigue_ += dna_["fatigue_turn_increase"];
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
		Think();
		fatigue_ += dna_["fatigue_turn_increase"];
/*		if (fatigue >= dna_["sleep_treshold"]) Sleep();
		else Think();		/**/
	}
}

void Unit::WakeUp() {
	sleep_ = false;
};

size_t Unit::Think(const double intelligence, std::shared_ptr<Unit> attacker) {
//	speed_ = 0.5 * dna_["speed"] + 0.25 * dna_["agility"] + 0.125 * dna_["intelligence"] + 0.125 * dna_["strength"];
//	double steps =
	const double pushed_intelligence = dna_["intelligence"];
	if (!isnan(intelligence)) dna_["intelligence"] = intelligence;
	DnaCode &dna = dna_;
	if (!!attacker) {
		double target_speed = 0.45 * speed_ + 0.45 * dna["agility"] + 0.1 * dna["intelligence"];
		double attacker_speed = 0.45 * attacker->speed_ + 0.45 * attacker->dna_["agility"] + 0.1 * attacker->dna_["intelligence"];
		double target_strength = 0.1 * (0.45 * dna["strength"] + 0.45 * dna["agility"] + 0.1 * speed_);
		double attacker_strength = 0.1 *(0.45 * attacker->dna_["strength"] + 0.45 * attacker->dna_["agility"] + 0.1 * attacker->speed_);
		if (target_speed >= attacker_speed) { //can run
			if (target_strength < attacker_strength) {
				// run and return
				if (!isnan(intelligence)) dna_["intelligence"] = pushed_intelligence;
				return 0;
			}
		}
		// fight
		double target_health = health_;
		double attacker_health = attacker->health_;
		std::gamma_distribution<double> target_distribution(2.0, pow(dna["intelligence"] / 100.0, 1.2));
		std::gamma_distribution<double> attacker_distribution(2.0, pow(attacker->dna_["intelligence"] / 100.0, 1.2));
		while (target_health > 0.0 || attacker_health > 0.0) {
			attacker_health -= target_distribution(world->generator);
			target_health -= attacker_distribution(world->generator);
		}
		if (target_health < 0.0) world->Kill(this);
		else health_ = target_health;
		if (attacker_health < 0.0) /*world->Kill(*attacker)/**/;
		else attacker->health_ = attacker_health;
		if (!isnan(intelligence)) dna_["intelligence"] = pushed_intelligence;
		return 0;
	}


	// An unit has to decide: eat or mate? If mate.
	// If mate, it has to decide which unit it wants to mate with.
	// Else it has to decide what to eat (what to attack).

	// steps to move
	//double steps = ;

/*
	double *first_need;
	double *second_need
	double *third_need;
	double hunger_normal = energy_ / dna["requirements.normal_turn"];
	double hunger_sleep = energy_ / dna["requirements.sleep_turn"];
	double fatigue = fatigue_ / dna["fatigue_turn_increase"];
	double fatigue_factor = dna["fatigue_turn_increase"] / 100.0;

	double predicting_factor = 0.8 * dna["intelligence"] + 0.2 * dna["speed"];
	double desired_calculated_speed = 110.0 / predicted_factor;
	double maximum_weight = desired_calculated_speed * dna["normal_weight"];
	double maximum_energy = maximum_weight - dna["normal_weight"];
	if (fatigue_ + dna["fatigue_turn_increase"] >= dna["fatigue_death_treshold"]) Sleep();
	else {
        if (energy_ < dna["normal_weight"]) {
			//find food
        }
        else {
			//find partner
        }
	}


	std::list<std::pair<size_t, double>> priority;
	//hunger
	priority.push_back(std::make_pair(UnitAction::Eating, energy_ / dna["requirements.normal_turn"])); // * some factor?
	//mating
	priority.push_back();/**/
}
