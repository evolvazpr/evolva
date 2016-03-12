#include "DnaGenerator.hpp"

std::shared_ptr<DnaCode> DnaGenerator::Generate() const {
	std::shared_ptr<DnaCode> dna = std::make_shared<DnaCode>();
	std::normal_distribution<double> variation(100.0, variability_);
	for (auto i = dna_code_->begin(); i != dna_code_->end(); ++i) {
		double variation_factor = variation(world->generator);
		if (variation_factor < 0.0) variation_factor *= -1.0;
		dna->insert(std::make_pair(i->first, i->second * variation_factor * 0.01));
	}
	return std::move(dna);
}
