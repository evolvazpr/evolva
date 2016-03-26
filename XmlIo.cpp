#include "XmlIo.hpp"

XmlIoElement::XmlIoElement(TiXmlNode *node) throw (EvolvaException) : actual_node_(node) {
	if (!actual_node_)
		throw EvolvaException("XmlIoElement error: tryied to create XmlIoElement pointing to null.\n");
}

XmlIoElement& XmlIoElement::operator[](const char *element) throw (EvolvaException) {
	TiXmlNode *temp;
	temp = actual_node_->FirstChild(element);
	if (temp == NULL)
		throw EvolvaException("XmlIoElement error: Tried to modify XmlIoElement, so that would point to null.\n"); 
	actual_node_ = temp;
	return *this;
}

XmlIo::XmlIo(const std::string path) throw (EvolvaException) : doc_(path.c_str()) {
	bool ret = doc_.LoadFile();
	if (!ret) throw EvolvaException("XmlIo fails: " + std::string(doc_.ErrorDesc()));
}

XmlIo::~XmlIo() {
	doc_.SaveFile();
}

XmlIoElement XmlIo::operator[](const char *element) throw (EvolvaException) {
	XmlIoElement node_(doc_.RootElement()->FirstChild(element));
	return node_; 
}

XmlIoElement::operator double() {
	const std::string text = actual_node_->FirstChild()->ValueStr();	
	try {
	double ret = std::stod(text);
	return ret;
	} catch(std::exception& exception) {
		throw EvolvaException("XmlIo fails: Tried to assign text to value of type double");
	}
}	

