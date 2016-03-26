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

/* 
 * Creating elements. Example of usage:
 * xml.CreateElements({"New_node", "Something_new"});
 * Effect:
 * <Evolva>
 *	(...)
 *	<New_node>
 *		<Something_new />
 *	</New_node>
 *	(...)
 *</Evolva>
 * Method can not initialize element with value.
 */
void XmlIo::CreateElement(std::initializer_list<const std::string> name_path) {
	TiXmlNode *node = doc_.RootElement();
	TiXmlNode *temp = nullptr;
	for (auto name : name_path) {
		temp = node->FirstChild(name);
		if(!temp) 
			node = node->InsertEndChild(TiXmlElement(name));
		else 
			node = temp;
	}
}
