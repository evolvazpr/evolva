#ifndef _DNA_GENERATOR_HPP_
#define _DNA_GENERATOR_HPP_

#include "DnaUnit.hpp"

class DnaGenerator : public DnaUnit {
public:
	using DnaUnit::DnaUnit;
	using DnaUnit::dna_;
	double variability_;
	std::shared_ptr<DnaCode> Generate() const;
};


#endif // _DNA_GENERATOR_HPP_
