#include "XmlIo.hpp"

int main(void) {
	XmlIo xml("test.xml\0");
	xml.GetValue();
	return 0;
}
