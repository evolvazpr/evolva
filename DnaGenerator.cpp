#include "Field.hpp"
#include "DnaGenerator.hpp"

std::shared_ptr<DnaCode> DnaGenerator::Generate() const {
	std::shared_ptr<DnaCode> dna = std::make_shared<DnaCode>();
	// Variability is deviation for normal distribution.
	// It is good to keep it in reasonable values so
	// variability is <0, 100> and this simple math below
	// calculates proportion, so 100 becomes 35.
	std::normal_distribution<double> variation(100.0, 7.0 * variability_ / 20.0);
	for (auto i = dna_code_->begin(); i != dna_code_->end(); ++i) {
		double variation_factor = variation(field->Random());
		if (variation_factor < 0.0) variation_factor *= -1.0;
		dna->insert(std::make_pair(i->first, i->second * variation_factor * 0.01));
	}
	return std::move(dna);
}
