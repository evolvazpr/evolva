#include <string>
#include "EvolvaException.hpp"
#include "XmlIo.hpp"

int main(void) {
	try {
		double test;
		XmlIo xml("test.xml\0");
		test = xml["Field"]["Width"];
		std::cout << "Field width: " << test << "\n";
		xml["Field"]["Width"] = 2262;
		test = xml["Field"]["Width"];
		std::cout << "Field width: " << test << "\n";
		xml.CreateElement({"a", "b", "c", "d"});
		xml.CreateElement({"empty"});
		xml["empty"] = 5;
		xml.SaveFile();
	} catch (std::exception& exception) {
		std::cout << exception.what() << std::endl;
	}
		return 0;
}
