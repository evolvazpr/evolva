#ifndef _EVOLVA_EXCEPTION_HPP
#define _EVOLVA_EXCEPTION_HPP

#include <exception>
#include <string>

class EvolvaException : public std::exception
{
	private:
		std::string warning_;
	public:
		EvolvaException(std::string warning) {
			std::swap(warning_, warning);
		}
	virtual const char* what() const throw() {
		return warning_.c_str();
	}
	~EvolvaException() {

	}
};

#endif //_EVOLVA_EXCEPTION_HPP
