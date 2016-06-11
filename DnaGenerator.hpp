#ifndef _DNA_GENERATOR_HPP_
#define _DNA_GENERATOR_HPP_

// includes
#include "DnaUnit.hpp"

/*< DnaGenerator is a handler for DNA generation settings,
it also provides generation. */
class DnaGenerator : public DnaUnit {
public:
	using DnaUnit::DnaUnit;
	using DnaUnit::dna_;
	/*< Generates new dna codes based on given codes.
	Codes are accessable as fields of class.*/
	std::shared_ptr<DnaCode> Generate() const;
	double variability_;
};

#endif // _DNA_GENERATOR_HPP_
