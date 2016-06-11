#include <string>
#include "../EvolvaException.hpp"
#include "../XmlIo.hpp"
#include <iostream>
#include <cstdlib>
#include<fstream>

int print_start() {
	char buf[100];
	std::ifstream file;
	std::cout << "\nThis is a test for XmlIo class.\n\nIt opens test.xml file, "	
	"writes into its nodes, changes value of Field - Width element,\n"
	"and saves file.\ntest.xml file now contains:\n\n";
	
	file.open("test.xml");
	if (!file.is_open())
		return -1;
	while(!file.eof()) {
		file >> buf;
		std::cout << buf;
		std::cout << "\n";
	}
	std::cout << "\n";
	file.close();
	return 0;
}

int print_end() {
	char buf[100];
	std::ifstream file;
	std::cout << "\nAt exit test.xml contains:\n\n";
	
	file.open("test.xml");
	if (!file.is_open())
		return -1;
	while(!file.eof()) {
		file >> buf;
		std::cout << buf;
		std::cout << "\n";
	}
	std::cout << "\n";
	
	file.close();
	return 0;
}

int main(void) {
	int ret;
	try {
		double test;
		
		if(print_start())
			return -1;
		
		ret = system("cp ./test.xml ./OLDtest.xml");
		XmlIo xml("test.xml\0");
		test = xml["Field"]["Width"];
		std::cout << "Field width: " << test << "\n";
		xml["Field"]["Width"] = 10;
		xml.CreateElement({"a", "b", "c", "d"});
		xml.CreateElement({"not_empty"});
		xml["not_empty"] = 5;
		xml.SaveFile();
		
		if(print_end()) 
			ret = -2;
		ret = system("mv ./OLDtest.xml ./test.xml");
		
	} catch (std::exception& exception) {
		std::cout << exception.what() << std::endl;
	}
		return ret;
}
