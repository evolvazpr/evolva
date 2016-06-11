#ifndef _UNIT_HPP_
#define _UNIT_HPP_

#include "CellObject.hpp"
#include "DnaUnit.hpp"

class Unit : public DnaUnit, public CellObject {
friend class Field;
public:
	/*< Constructor */
	Unit();
	/*< Constructor with DNA specified. */
	Unit(std::shared_ptr<DnaCode> dna_code);
	/*< Destructor */
	virtual ~Unit();
	/*< Returns current energy. */
	inline double GetEnergy() const { return energy_; };
	/*< Returns current fatigue. */
	inline double GetFatigue() const { return fatigue_; };
	/*< Check is unit alive. */
	inline bool IsAlive() const { return alive_; };
	/*< Return unit's move priority. */
	double GetMovePriority() const;
	/*< Same as GetMovePriority(). */
	bool operator <= (const std::shared_ptr<Unit> object) const;
	
private:
	/*< Updates current status, checks death time. */
	void Update();
	/*< Unit falls asleep. */
	void Sleep();
	/*< Wakes up from sleep. */
	void WakeUp();
	void UpdateSpeed();
	void CalculateDeathTime();
	size_t Think(std::shared_ptr<Unit> attacker = nullptr);
	bool Pregnant(std::shared_ptr<DnaCode> dna);
	size_t Explore(double steps);
	inline std::shared_ptr<Unit> GetUnit(){ return std::static_pointer_cast<Unit>(shared_from_this()); };
	virtual bool Move(const int x, const int y, const bool trim = false);
	template <class SC, class F> std::shared_ptr<SC> Action(double steps_limit, double max_steps, double radius, double efficiency, F conditions);
	void GiveBirth(const int x, const int y);
	void Miscarry();
	inline bool IsCarnivore() const { return dna_["carnivore"] >= 50.0; };
	inline bool IsHerbivore() const { return dna_["herbivore"] >= 50.0; };
	void RemoveStatistics();
	size_t turns_;										/*< Current lifetime in turns */
	bool sleep_;										/*< Is unit sleeping. */
	double fatigue_;									/*< Level of fatigue (0 - 100). */
	double health_;										/*< Level of health (0 - 100). */
	double speed_;										/*< Calculated level of speed (0 - 100). */
	double attractiveness_;								/*< Calculated level of attractiveness (0 - 100). */
	double strength_;									/*< Calculated level of strength. */
	double energy_;										/*< Level stored in nutritions in organism. */
	size_t death_;
	bool pregnant_;
	std::shared_ptr<DnaCode> child_dna_code_;
	size_t pregnant_turns_;
	bool puerperium_;
	bool alive_;
	size_t death_reason_;
	bool omnivore_;
	bool disabled_;
	
	Unit(const Unit&) = delete;
	operator = (const Unit&) = delete;
};

#endif // _UNIT_HPP_
