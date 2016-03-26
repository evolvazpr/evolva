#ifndef _XMLIO_HPP_
#define _XMLIO_HPP_

#include <string>
#include <sstream>
#include <initializer_list>
#include "tinyxml/tinyxml.h"
#include "EvolvaException.hpp"

class XmlIoElement {
	private:
		XmlIoElement() = delete;
		TiXmlNode* actual_node_;
	public:
		XmlIoElement(TiXmlNode *node) throw (EvolvaException);
		XmlIoElement& operator[](const std::string element) throw (EvolvaException);
		template <class T> operator T() const;
		template <class T> XmlIoElement& operator =(const T value);
};

template <class T> XmlIoElement& XmlIoElement::operator =(const T value) {
	TiXmlNode *old_value = actual_node_->FirstChild();
	std::stringstream stream;
	stream << value;
	if (old_value)
		actual_node_->ReplaceChild(old_value, TiXmlText(stream.str()));
	else
		actual_node_->InsertEndChild(TiXmlText(stream.str()));
	return *this;
}

template <class T> XmlIoElement::operator T() const {
	std::string text = actual_node_->FirstChild()->ValueStr();
	std::stringstream stream(text);
	T ret;
	stream >> ret;
	return ret;
}

class XmlIo {
	private:
		XmlIo() = delete;
		TiXmlDocument doc_;	
	public:
		XmlIo(const std::string path) throw (EvolvaException);
		~XmlIo();
		XmlIoElement operator[](const std::string element) throw (EvolvaException);
		void CreateElement(std::initializer_list<const std::string> name_path);
};

#endif //_XMLIO_HPP_
