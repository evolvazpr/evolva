#include "XmlIo.hpp"

XmlIo::XmlIo(const std::string path) throw (EvolvaException) : doc_(path.c_str()) {
	bool ret = doc_.LoadFile();
	if (!ret) throw EvolvaException("XmlIo fails: " + std::string(doc_.ErrorDesc()));
	actual_node_ = doc_.RootElement();
}

XmlIo::~XmlIo() {
	doc_.SaveFile();
}

XmlIo& XmlIo::operator[](const char *element) throw (EvolvaException) {
	actual_node_ = actual_node_->FirstChild(element);
	if (!actual_node_) {
		actual_node_ = doc_.RootElement();
		throw EvolvaException("XmlIo fails: there is no \"" + std::string(element) + "\" element.\n");
	}
	return *this;
}

XmlIo::operator double() {
	const std::string text = actual_node_->FirstChild()->ValueStr();	
	size_t counter;
	double ret = std::stod(text, &counter);
	actual_node_ = doc_.RootElement();
	if (!counter)
		throw EvolvaException("XmlIo fails: Tried to assign text to value of type double");
	return ret;
}	

const std::string XmlIo::GetData(const std::string first, const std::string second) const throw (EvolvaException) {
	const TiXmlNode* root = doc_.RootElement();
	const TiXmlNode* xml_element = root->FirstChild(first.c_str());
	
	if (!xml_element)
		throw EvolvaException("XmlIo fails: There is no element: \"" + first + "\".\n") ;
	xml_element = xml_element->FirstChild(second.c_str());
	if (!xml_element)
		throw EvolvaException("XmlIo fails: There is no element \"" + second + "\" in \"" + first + "\".\n");
	return xml_element->FirstChild()->ValueStr();	
}

void XmlIo::WriteData(const std::string first, const std::string second, const std::string text) {
	TiXmlElement *root = doc_.RootElement();
	TiXmlNode *element = root->FirstChild(first.c_str());
	if (element) { //first element exists, do not create the same element.
		element = element->FirstChild(second.c_str());
	       if (element) { //second element exists, replace texts
			element->ReplaceChild(element->FirstChild(), TiXmlText(text));	
	       } else {	 //second element does not exists, add element and text	
			//reassigning pointer, because variable was used few lines ago.
			element = root->FirstChild(first.c_str()); 	
			element = element->InsertEndChild(TiXmlElement(second));
			element->InsertEndChild(TiXmlText(text));
		}	       
	} else { //the first element does not exists, hence also latter elements. Create everything.
		element = root->InsertEndChild(TiXmlElement(first));
		element = element->InsertEndChild(TiXmlElement(second));
		element = element->InsertEndChild(TiXmlText(text));
	}
}
