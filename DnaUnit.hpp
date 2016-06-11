#ifndef _DNA_UNIT_HPP_
#define _DNA_UNIT_HPP_

// includes
#include <memory>
#include <string>
#include <unordered_map>

// important typedefs
typedef std::unordered_map<std::string, double> DnaCode;

/*< DnaUnit is a base class for every units with dna. */
class DnaUnit {
protected:
	std::shared_ptr<DnaCode> dna_code_;
public:
	DnaCode &dna_;
	DnaUnit();
	DnaUnit(std::shared_ptr<DnaCode> dna_code);
	virtual ~DnaUnit();
	inline double GetDna(const std::string &entry) { return dna_[entry]; };
	std::shared_ptr<DnaCode> Mating(const std::shared_ptr<const DnaCode> dna_in) const;
private:
	DnaUnit(const DnaUnit&) = delete;
	DnaUnit& operator = (const DnaUnit&) = delete;
};

#endif // _DNA_UNIT_HPP_
