#ifndef _XMLIO_HPP_
#define _XMLIO_HPP_

#include <string>

#include "tinyxml/tinyxml.h"
#include "EvolvaException.hpp"

class XmlIoElement {
	private:
		XmlIoElement() = delete;
		TiXmlNode* actual_node_;
	public:
		XmlIoElement(TiXmlNode *node) throw (EvolvaException);
		XmlIoElement& operator[](const char *element) throw (EvolvaException);
		operator double();
		template <class T> XmlIoElement& operator =(const T value);
};

template <class T> XmlIoElement& XmlIoElement::operator =(const T value) {
	TiXmlNode *old_value = actual_node_->FirstChild();
	if (old_value) 
		actual_node_->ReplaceChild(old_value, TiXmlText(std::to_string(value)));
	else 
		actual_node_->InsertEndChild(TiXmlText(TiXmlText(std::to_string(value))));
	return *this;
}

class XmlIo {
	private:
		XmlIo() = delete;
		TiXmlDocument doc_;
		
	public:
		XmlIo(const std::string path) throw (EvolvaException);
		~XmlIo();
		XmlIoElement operator[](const char* element) throw (EvolvaException);
};


#endif //_XMLIO_HPP_
