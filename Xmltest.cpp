#include <string>
#include "XmlIo.hpp"

int main(void) {
	XmlIo xml("test.xml\0");
	std::cout << "Field: Width: " << xml.GetValue("Field", "Width") << std::endl;
	std::cout << "Field: Length: " << xml.GetValue("Field", "Length") << std::endl;
	return 0;
}
