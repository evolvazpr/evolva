#ifndef _EVOLVA_EXCEPTION_HPP_
#define _EVOLVA_EXCEPTION_HPP_

#include <exception>
#include <string>

class EvolvaException : public std::exception
{
	private:
		const char* warning_;
	public:
		EvolvaException(const char*&& warning) {
			warning_ = warning;
		}
	virtual const char* what() const throw() {
		return warning_;
	}
};

#endif //_EVOLVA_EXCEPTION_HPP_
