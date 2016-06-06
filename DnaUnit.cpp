#include "Field.hpp"
#include "DnaUnit.hpp"

DnaUnit::DnaUnit() : dna_(*dna_code_.get()), dna_code_(std::make_shared<DnaCode>()) {
}

DnaUnit::DnaUnit(std::shared_ptr<DnaCode> dna_code) : dna_(*dna_code_.get()), dna_code_(dna_code) {
}

DnaUnit::~DnaUnit() {
}

std::shared_ptr<DnaCode> DnaUnit::Mating(const std::shared_ptr<const DnaCode> dna_in) const {
	auto dna = std::make_shared<DnaCode>();
	// crossover and mutating
	// mutation genes (m. genes affect only children, not parents)
	dna->insert(std::make_pair("mutability", 0.5 * (dna_code_->at("mutability") + dna_in->at("mutability"))));
	dna->insert(std::make_pair("mutation_stability", 0.5 * (dna_code_->at("mutation_stability") + dna_in->at("mutation_stability"))));
	// other genes
	for (auto i = dna_code_->begin(); i != dna_code_->end(); ++i) {
		// crossover
		if (i->first == "mutability" || i->first == "mutation_stability") continue;
		dna->insert(std::make_pair(i->first, 0.5 * (i->second + dna_in->at(i->first))));
		// mutating
		std::normal_distribution<double> mutation(100.0, field->Mutability() * dna->at("mutability"));
		double mutation_factor = mutation(field->Random()) * 0.01;
		if (mutation_factor < 0.0) mutation_factor *= -1.0;
		if (mutation_factor >= dna->at("mutation_stability")) dna->at(i->first) = dna->at(i->first) * mutation_factor;
	}
	// returning
	return dna;
}
