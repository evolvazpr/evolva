#include <string>
#include "../EvolvaException.hpp"
#include "../XmlIo.hpp"

int main(void) {
	try {
		double test;
		XmlIo xml("test.xml\0");
		test = xml["Field"]["Width"];
		std::cout << "Field width: " << test << "\n";
		xml["Field"]["Width"] = 10;
		test = xml["Field"]["Width"];
		std::cout << "Field width: " << test << "\n";
		xml.CreateElement({"a", "b", "c", "d"});
		xml.CreateElement({"not_empty"});
		xml["not_empty"] = 5;
		xml.SaveFile();
	} catch (std::exception& exception) {
		std::cout << exception.what() << std::endl;
	}
		return 0;
}
