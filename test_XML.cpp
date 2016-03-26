#include <string>
#include "EvolvaException.hpp"
#include "XmlIo.hpp"

int main(void) {
	try {
		double test;
		XmlIo xml("test.xml\0");
		test = xml["Field"]["Width"];
		std::cout << "Field width: " << test << "\n";
		xml["Field"]["Width"] = 2;
		test = xml["Field"]["Width"];
		std::cout << "Field width: " << test << "\n";
		test = xml["Field"]["TEST"];
		std::cout << "Field TEST: " << test << "\n";
		test = xml["DUPA_NIE_UDA"];
	} catch (std::exception& exception) {
		std::cout << exception.what() << std::endl;
	}
		return 0;
}
