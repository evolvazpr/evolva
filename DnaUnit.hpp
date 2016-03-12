#ifndef _DNA_UNIT_HPP
#define _DNA_UNIT_HPP

#include <string>
#include <memory>
#include <unordered_map>

typedef std::unordered_map<std::string, double> DnaCode;

class DnaUnit {
public:
	DnaUnit();
	virtual ~DnaUnit();
protected:
	DnaUnit(std::shared_ptr<DnaCode> dna_code);
	std::shared_ptr<DnaCode> dna_code_;
	DnaCode &dna_;
	std::string dumpDna();
	std::shared_ptr<DnaCode> Mating(const std::shared_ptr<const DnaCode> dna_in) const ;
};

#endif // _DNA_UNIT_HPP
