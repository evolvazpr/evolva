#ifndef _UNIT_HPP_
#define _UNIT_HPP_

#include "CellObject.hpp"
#include "DnaUnit.hpp"
#include <cmath>
#include <vector>

class Unit : public DnaUnit, public Creature {
friend class Field;
public:
	Unit();
	Unit(std::shared_ptr<DnaCode> dna_code);
	virtual ~Unit();
	inline double GetEnergy() const { return energy_; };
	inline double GetFatigue() const { return fatigue_; };
	inline bool IsAlive() const { return alive_; };
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
		double attractiveness_;								/*< Calculated level of attractiveness (0 - 100). */
		double strength_;									/*< Calculated level of strength. */
		double energy_;										/*< Level stored in nutritions in organism. */
		double poison_;										/*< Level of poison. */
		size_t death_;
		bool pregnant_;
		std::shared_ptr<DnaCode> child_dna_code_;
		size_t pregnant_turns_;
		bool puerperium_;
	/*< Updates current status, checks death time. */
	void Update();
	void Sleep();
	/*< Wakes up from sleep. */
	void WakeUp();
	void UpdateSpeed();
	void CalculateDeathTime();
	//! TODO: intelligence? swarm? :(
	size_t Think(const double intelligence = NAN, std::shared_ptr<Unit> attacker = nullptr);
	virtual double GetMovePriority() const;
	bool Pregnant(std::shared_ptr<DnaCode> dna);
	size_t Explore(double steps);
	inline std::shared_ptr<Unit> GetUnit(){ return std::static_pointer_cast<Unit>(shared_from_this()); };
	bool alive_;
	virtual bool Move(const int x, const int y, const bool trim = false);
	template <class SC, class F> std::shared_ptr<SC> Action(double steps_limit, double max_steps, double radius, double efficiency, F conditions);
	void GiveBirth(const int x, const int y);
	void Miscarry();
};

#endif // _UNIT_HPP_
