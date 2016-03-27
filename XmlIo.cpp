#include "XmlIo.hpp"

XmlIo::Element::Element(TiXmlNode *node) throw (EvolvaException) : actual_node_(node) {
	if (!actual_node_)
		throw EvolvaException("XmlIoElement error: tryied to create XmlIoElement pointing to null.\n");
}

XmlIo::Element& XmlIo::Element::operator[](const std::string element) throw (EvolvaException) {
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

XmlIo::Element XmlIo::operator[](const std::string element) throw (EvolvaException) {
	Element node_(doc_.RootElement()->FirstChild(element));
	return node_; 
}
