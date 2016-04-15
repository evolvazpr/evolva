#include <string>
#include "../EvolvaException.hpp"
#include "../XmlIo.hpp"
#include <iostream>
#include <cstdlib>

void print_test() {
	std::cout << "\nThis is a test for XmlIo class.\n\nIt opens test.xml file, "	
		"writes into it nodes, changes value of Field - Width element,\n"
		"and saves file.\ntest.xml file now contains:\n\n";
	system("cat test.xml");
	std::cout << "\n\n";
}

void print_end() {
	std::cout << "\nAt exit test.xml contains:\n\n";
	system("cat test.xml");
	std::cout << "\n\n";
}

int main(void) {
	try {
		double test;
		print_test();
		XmlIo xml("test.xml\0");
		test = xml["Field"]["Width"];
		std::cout << "Field width: " << test << "\n";
		xml["Field"]["Width"] = 10;
		xml.CreateElement({"a", "b", "c", "d"});
		xml.CreateElement({"not_empty"});
		xml["not_empty"] = 5;
		xml.SaveFile();
		print_end();
	} catch (std::exception& exception) {
		std::cout << exception.what() << std::endl;
	}
		return 0;
}
