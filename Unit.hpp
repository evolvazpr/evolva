#ifndef _UNIT_HPP_
#define _UNIT_HPP_

#include "CellObject.hpp"
#include "DnaUnit.hpp"
#include <cmath>

class Unit : public DnaUnit, public NonPlant {
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
	void UpdateSpeed();
	void CalculateDeathTime();
	size_t Think(const double intelligence = NAN, std::shared_ptr<Unit> attacker = nullptr);
	virtual double GetMovePriority() const;
};

#endif // _UNIT_HPP_
