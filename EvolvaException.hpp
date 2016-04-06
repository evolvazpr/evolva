#ifndef _EVOLVA_EXCEPTION_HPP
#define _EVOLVA_EXCEPTION_HPP

#include <exception>
#include <string>
/**\brief Evolva exception class*/
class EvolvaException : public std::exception
{
	private:
		std::string message_;
	public:
		/**
		 * \param message - contains information
		 * about exception.
		 */
		EvolvaException(std::string message) {
			std::swap(message_, message);
		}
	/**
	 * \ret const char pointer with exception's message
	 */
	virtual const char* what() const throw() {
		return message_.c_str();
	}
	~EvolvaException() {

	}
};

#endif //_EVOLVA_EXCEPTION_HPP
