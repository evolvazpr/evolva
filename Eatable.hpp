#ifndef _EATABLE_HPP_
#define _EATABLE_HPP_

class Eatable : public NonMovableObject {
public:
	inline double GetDefaultEnergy() const { return default_energy_; };
	inline double GetEnergy() const { return energy_; };
	inline size_t GetAge() const { return turns_; };
	void Grow();
	double Eat(double energy);
	Eatable(const double defualt_energy, const double growth_a, const double growth_b, const double growth_c, const size_t growth_treshold);
	virtual ~Eatable();
protected:
	double default_energy_;
	double energy_;
	size_t turns_;
	// Growth a, b and c are equation factors.
	// growth = default_energy * (a * turns^2 + b * turns + c)
	// Growth affects amount of energy. Can be negative.
	// growth_treshold is the end point of growth
	double growth_a_;
	double growth_b_;
	double growth_c_;
	size_t growth_treshold_;
};

#endif //_EATABLE_HPP_
