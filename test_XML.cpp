#include <string>
#include "EvolvaException.hpp"
#include "XmlIo.hpp"

int main(void) {
	try {
		double test;
		XmlIo xml("test.xml\0");
		xml.WriteData("Field", "Width", "555");
		xml.WriteData("Field", "TEST", "2");
		xml.WriteData("NOWE", "NOWSZE2", "DODA SIE?");
		test = xml["Field"]["Width"];
		std::cout << "Field width: " << test << "\n";
		test = xml["Field"]["TEST"];
		std::cout << "Field TEST: " << test << "\n";
		test = xml["NOWE"];
	} catch (std::exception& exception) {
		std::cout << exception.what() << std::endl;
	}
		return 0;
}
