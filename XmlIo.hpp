#ifndef _XMLIO_HPP_
#define _XMLIO_HPP_

#include <string>
#include <sstream>
#include <initializer_list>
#include "tinyxml/tinyxml.h"
#include "EvolvaException.hpp"

class XmlIo {
	private:
		XmlIo() = delete;
		TiXmlDocument doc_;	

		class Element {
			private:
				Element() = delete;
				TiXmlNode* actual_node_;
			public:
				Element(TiXmlNode *node) throw (EvolvaException);
				Element& operator[](const std::string element) throw (EvolvaException);
				template <class T> operator T() const;
				template <class T> Element& operator =(const T value);
		};
	public:
		XmlIo(const std::string path) throw (EvolvaException);
		~XmlIo();
		Element operator[](const std::string element) throw (EvolvaException);
		template <class T = int> void CreateElement(std::initializer_list<const std::string> name_path, T value = 0);
};

template <class T> XmlIo::Element& XmlIo::Element::operator =(const T value) {
	TiXmlNode *old_value = actual_node_->FirstChild();
	std::stringstream stream;
	stream << value;
	TiXmlText text(stream.str());
	if (old_value)
		actual_node_->ReplaceChild(old_value, text);
	else
		actual_node_->InsertEndChild(text);
	return *this;
}

template <class T> XmlIo::Element::operator T() const {
	std::string text = actual_node_->FirstChild()->ValueStr();
	std::stringstream stream(std::move(text));
	T ret;
	stream >> ret;
	return ret;
}

/* 
 * Creating elements. Example of usage:
 * xml.CreateElements({"New_node", "Something_new"}, 5);
 * Effect:
 * <Evolva>
 *	(...)
 *	<New_node>
 *		<Something_new>5</Something_new>
 *	</New_node>
 *	(...)
 *</Evolva>
 *
 * Method can be invoked without second argument (second argument is 0 as default)
 */
template <class T> void XmlIo::CreateElement(std::initializer_list<const std::string> name_path, T value) {
	TiXmlNode *node = doc_.RootElement();
	TiXmlNode *temp = nullptr;
	std::stringstream stream;
	stream << value;
	TiXmlText text(stream.str());
	for (auto name : name_path) {
		temp = node->FirstChild(name);
		if(!temp) 
			node = node->InsertEndChild(TiXmlElement(std::move(name)));
		else 
			node = temp;
	}
	/*If there was last element - replace values*/
	if (temp) node->ReplaceChild(node->FirstChild(), std::move(text)); //To move or not to move, that is a question.
	/*If there was no last element - insert new value*/
	else node->InsertEndChild(text);	
}

#endif //_XMLIO_HPP
