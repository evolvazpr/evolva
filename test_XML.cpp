#include <string>
#include "EvolvaException.hpp"
#include "XmlIo.hpp"

int main(void) {
	try {
		XmlIo xml("test.xml\0");
		std::cout << "Field: Width: " << xml.GetData("Field", "Width") << std::endl;
		std::cout << "Field: Length: " << xml.GetData("Field", "Length") << std::endl;
		xml.WriteData("Field", "Width", "555");
		xml.WriteData("Field", "TEST", "2");
		xml.WriteData("NOWE", "NOWSZE2", "DODA SIE?");
	} catch (EvolvaException& exception) {
		std::cout << exception.what();
	}
		return 0;
}
