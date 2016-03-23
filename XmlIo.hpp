#ifndef _XMLIO_HPP_
#define _XMLIO_HPP_

#include <string>

#include "tinyxml/tinyxml.h"
#include "EvolvaException.hpp"

class XmlIo {
	private:
		XmlIo() = delete;
		TiXmlDocument doc_;

	public:
		XmlIo(const std::string path) throw (EvolvaException);
		~XmlIo();

		const std::string GetData(const std::string first, const std::string second) 
			const throw (EvolvaException);
		void WriteData(const std::string first, const std::string second, const std::string text);
};

#endif //_XMLIO_HPP_
