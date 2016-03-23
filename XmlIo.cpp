#include "XmlIo.hpp"

XmlIo::XmlIo(const std::string path) throw (EvolvaException) : doc_(path.c_str()) {
	LoadFile();
}

void XmlIo::LoadFile() throw (EvolvaException) {
	bool ret = doc_.LoadFile();
	if (!ret) throw EvolvaException("XmlIo fails: " + std::string(doc_.ErrorDesc()));
}

const std::string XmlIo::GetData(const std::string first, const std::string second) const throw (EvolvaException) {
	const TiXmlNode* root = doc_.RootElement();
	const TiXmlNode* xml_object = root->IterateChildren(nullptr);
	const TiXmlNode* xml_element;
	while (xml_object) {
		if (xml_object->ValueStr() == first) {
			xml_element = xml_object->IterateChildren(nullptr);
			while (xml_element) {
				if (xml_element->ValueStr() == second) return xml_element->FirstChild()->ValueStr();
				else xml_element = xml_object->IterateChildren(xml_element);
			}
			throw EvolvaException("XmlIo fails: There is no element \"" + second + "\" in \"" + first + "\".\n");
		} else {
			xml_object = root->IterateChildren(xml_object);
		}
	}
	throw EvolvaException("XmlIo fails: There is no element: \"" + first + "\".\n") ;
}

void XmlIo::WriteData(const std::string first, const std::string second, const std::string text) {
	TiXmlElement first_element(first);
	TiXmlElement second_element(second);
	TiXmlText text_element(text);
	TiXmlElement *root = doc_.RootElement();
	second_element.InsertEndChild(text_element);
	first_element.InsertEndChild(second_element);	
	root->InsertEndChild(first_element);
	doc_.SaveFile();
}
