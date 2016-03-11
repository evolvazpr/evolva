#ifndef _EVOLVA_EXCEPTION_HPP
#define _EVOLVA_EXCEPTION_HPP

#include <exception>
#include <string>

class EvolvaException : public std::exception
{
	private:
		const char* warning_;
	public:
		EvolvaException(const char *warning) {
			std::swap<const char *>(warning_, warning);
		}
	virtual const char* what() const throw() {
		return warning_;
	}
};

#endif //_EVOLVA_EXCEPTION_HPP
