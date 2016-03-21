#include "XmlIo.hpp"

XmlIo::XmlIo(std::string path) throw (EvolvaException) : doc_(path.c_str()) {
	LoadFile();
}

bool XmlIo::LoadFile() throw (EvolvaException) {
	bool ret = doc_.LoadFile();
	if (!ret) throw EvolvaException("XmlIo fails: " + std::string(doc_.ErrorDesc()));
	else return true;
}

std::string XmlIo::GetValue(std::string first, std::string second) {
	TiXmlNode* root = doc_.RootElement();
	TiXmlNode* xml_object = root->IterateChildren(nullptr);
	TiXmlNode* xml_element;
	while (xml_object) {
		if (xml_object->ValueStr() == first) {
			xml_element = xml_object->IterateChildren(nullptr);
			while (xml_element) {
				if (xml_element->ValueStr() == second) return xml_element->FirstChild()->ValueStr();
				else xml_element = xml_object->IterateChildren(xml_element);
			}
			throw EvolvaException("XmlIo fails: Problem with element: \"" + second + "\" in element: \"" + first + "\".");
		} else {
			xml_object = root->IterateChildren(xml_object);
		}
	}
	return "\0";
}
