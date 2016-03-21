#ifndef _XMLIO_HPP_
#define _XMLIO_HPP_

#include <string>

#include "tinyxml/tinyxml.h"
#include "EvolvaException.hpp"

class XmlIo {
	private:
		XmlIo() = delete;
		TiXmlDocument doc_;
		bool LoadFile() throw (EvolvaException); 
	public:
		XmlIo(std::string path);
		TiXmlText* GetValue();
};

#endif //_XMLIO_HPP_
