#include "XmlIo.hpp"

XmlIo::XmlIo(std::string path) : doc_(path.c_str()) {
}

bool XmlIo::LoadFile() throw (EvolvaException) {
	bool ret = doc_.LoadFile();
	if (!ret) throw EvolvaException( std::string("XmlIo fails: ") + std::string(doc_.ErrorDesc()));
	else return true;
}

TiXmlText* XmlIo::GetValue() {
	LoadFile();
	TiXmlNode* root = doc_.RootElement();
	TiXmlNode* xml_object = root->IterateChildren(nullptr);
	TiXmlNode* xml_element;
	while (xml_object) {
		std::cout << "Object: " << xml_object->ValueStr() << std::endl;
		xml_element = xml_object->IterateChildren(nullptr);
		while (xml_element) {
			std::cout<< "		Element: " << xml_element->ValueStr() << std::endl;
			std::cout << "			Value: " << xml_element->FirstChild()->ValueStr() << std::endl;
			xml_element = xml_object->IterateChildren(xml_element);
		}
		xml_object = root->IterateChildren(xml_object);
	}
	return nullptr;
}
