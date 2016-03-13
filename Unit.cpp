#include "Field.hpp"
#include "Unit.hpp"

double Heaviside(const double x) { return (x <= 0.0 ? 0.0 : x); }

Unit::Unit() : DnaUnit(), Creature() {
	SetType(Type::UNIT, true);
	//std::cout << "UNIT!\n";
	// Death time calculating.
	//CalculateDeathTime();
}

Unit::Unit(std::shared_ptr<DnaCode> dna_code) : DnaUnit(dna_code), Creature() {
	SetType(Type::UNIT, true);
	CalculateDeathTime();
	energy_ = dna_["normal_weight"];
	UpdateSpeed();
}

void Unit::CalculateDeathTime() {
//    Unit *d = this;
	const double death_factor_u = dna_["death_time_u"] * (0.8 + 0.001 * ((dna_["agility"] + dna_["intelligence"] - dna_["speed"] - dna_["strength"])));
	const double death_factor_s = dna_["death_time_s"] - (dna_["agility"] + dna_["strength"] - dna_["intelligence"] - dna_["speed"]) / 240.0;
	std::normal_distribution<double> distribution(death_factor_u, death_factor_s);
	double death = distribution(field->Random());
	death_ = static_cast<size_t>(death - floor(death) >= 0.5 ? ceil(death) : floor(death));
}

void Unit::Update() {
	UpdateSpeed();
	++turns_;
	if (turns_ >= death_) field->Kill(this);
	if (fatigue_ >= dna_["fatigue_death_treshold"]) field->Kill(this);
	if (poison_ > 0.0) health_ -= dna_["poison_susceptibility"] * poison_;
	if (health_ <= dna_["health_death_treshold"]) field->Kill(this);
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
		if (fatigue_ <= dna_["wakeup_treshold"]) {
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
/*		if (fatigue >= dna_["sleep_treshold"]) Sleep();
		else Think();		/**/
	}
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

size_t Unit::Think(const double intelligence, std::shared_ptr<Unit> attacker) {
	// Calculate steps the unit can move. C. speed is <0, 100> (can be lower),
	// so when c. speed is 100, the unit is able to move 25 steps.
	double steps = static_cast<size_t>(Heaviside(speed_ / 4.0));
	// If intelligence is specified, method uses "swarm intelligence".
	const double pushed_intelligence = dna_["intelligence"];
	if (!isnan(intelligence)) dna_["intelligence"] = intelligence;
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
				// Run and return
				if (!isnan(intelligence)) dna_["intelligence"] = pushed_intelligence;
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
		if (!isnan(intelligence)) dna_["intelligence"] = pushed_intelligence;
		return 0;
	}

	// An unit has to decide: eat or mate? If mate.
	// If mate, it has to decide which unit it wants to mate with.
	// Else it has to decide what to eat (what to attack).
}


double Unit::GetMovePriority() const {
	return speed_;
}
