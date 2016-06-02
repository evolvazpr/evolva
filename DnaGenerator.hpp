#ifndef _DNA_GENERATOR_HPP_
#define _DNA_GENERATOR_HPP_

#include "DnaUnit.hpp"

class DnaGenerator : public DnaUnit {
public:
	using DnaUnit::DnaUnit;
	using DnaUnit::dna_;
	double variability_;
	/*< Generates new dna codes based on given codes.
	Codes are accessable as fields of class.*/
	std::shared_ptr<DnaCode> Generate() const;
};


#endif // _DNA_GENERATOR_HPP_
