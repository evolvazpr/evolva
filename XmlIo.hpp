#ifndef _XMLIO_HPP_
#define _XMLIO_HPP_

#include <string>
#include <sstream>
#include <initializer_list>
#include "tinyxml/tinyxml.h"
#include "EvolvaException.hpp"

/*brief XML document representation.
*/
class XmlIo {
	private:
		XmlIo() = delete;
		TiXmlDocument doc_;	

	public:
		/** XML's element (node) representation.
		 * Class represents temporary object created due to the call of XmlIo's operator [].
		 */
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

		XmlIo(const std::string path) throw (EvolvaException);
		~XmlIo();
		Element operator[](const std::string element) throw (EvolvaException);
		template <class T = int> void CreateElement(std::initializer_list<const std::string> name_path, T value = 0);
		void SaveFile();
};

/**\brief Assign operator template. Works with all standard numeric types. 
 *
 * Method changes value in specific xml's element. If value is present, method will replace it.
 * Example of usage:
 * \code{.cpp} xml["first_element"]["second_element"] = 5; \endcode
 *
 * \param value - value to be inserted into element.
*/
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

/**\brief Type operator template. Works with all standard types.
 *
 * Example of usage:
 * \code{.cpp} int x = xml["first_element"]["second_element"]; \endcode
*/
template <class T> XmlIo::Element::operator T() const {
	std::string text = actual_node_->FirstChild()->ValueStr();
	std::stringstream stream(std::move(text));
	T ret;
	stream >> ret;
	return ret;
}

/**\brief Creates one or more elements.
 *\tparam T - type of value passed to the operator, may be omitted. 
 *\param name_path - list - relationship of elements in xml file to be (re)written. Elements, which do not exist in xml file right now, will be written now to XmlIo object, and saved to XML document related to XmlIo object after invoking XmlIo::SaveFile method, or after deconstruction of XmlIo object.
 * 
 * \param value - value of last element passed in list. Basic types can be passed, because method is templated. If parameter is not passed, value defaults to 0. 
 *
 *
 * Node/element, which is last on the list, gets \p value. If \p value is omitted, variable defaults to 0. Method uses template to permit all basic types as second parameter.
 *
 * Example of usage:
 *	\code{.cpp}
 *	XmlIo xml("test.xml");
 *	xml.CreateElements({"New_node", "Something_new"}, 5);
 *	\endcode
 * Effect:
 *	\code{.xml}
 *	<evolva>
 *		(...)
 *		<New_node>
 *			<Something_new>5</Something_new>
 *		</New_node>
 *		(...)
 *	</evolva>
 *	\endcode
 * 
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
	// If there was last element - replace values
	if (temp) node->ReplaceChild(node->FirstChild(), text);
	// If there was no last element - insert new value
	else node->InsertEndChild(text);	
}

#endif //_XMLIO_HPP
