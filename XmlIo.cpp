#include "XmlIo.hpp"
/**
 * XmlIo::Element class, which objects are created in time of accessing xml's nodes via [] operator.
 * Method throws exception while passing nullptr as node parameter.
 *
 * \param node - pointer to node. It is needed by XmlIo::Element operator[] overload. If node is nullptr - exception will be thrown.
*/
XmlIo::Element::Element(TiXmlNode *node) throw (EvolvaException) : actual_node_(node) {
	if (!actual_node_)
		throw EvolvaException("XmlIoElement error: tryied to create XmlIoElement pointing to null.\n");
}


/**\brief Operator [] overload for node access. 
 * 
 * Method throws exception in time of attempt to access not existing node.
 * 
* \param element - name of element to be accessed.
*/
XmlIo::Element& XmlIo::Element::operator[](const std::string element) throw (EvolvaException) {
	TiXmlNode *temp;
	temp = actual_node_->FirstChild(element);
	if (temp == NULL)
		throw EvolvaException("XmlIoElement error: Tried to access not existing node.\n"); 
	actual_node_ = temp;
	return *this;
}


/**
 * Constructor throws exception when open of XML document fails. XML file must be prepared properly before constructing object of this class.
 * \param path - path to xml file.
*/
XmlIo::XmlIo(const std::string path) throw (EvolvaException) : doc_(path.c_str()) {
	bool ret = doc_.LoadFile();
	if (!ret) 
		throw EvolvaException("XmlIo fails: " + std::string(doc_.ErrorDesc()));
}

/**
 * Saves XML document (writes data to XML file) and destructs XmlIo object.
*/
XmlIo::~XmlIo() {
}


/**\brief Operator [] overload for node access. 
 *
 * Method throws exception in time of attempt to access not existing node.
 *
 * \param element - name of node to be accessed.
*/
XmlIo::Element XmlIo::operator[](const std::string element) throw (EvolvaException) {
	Element node_(doc_.RootElement()->FirstChild(element));
	return node_; 
}

/**
 * Saves XML document (writes data to XML file).
*/
void XmlIo::SaveFile() {
	doc_.SaveFile();
}
