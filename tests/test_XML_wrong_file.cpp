#include <string>
#include "../EvolvaException.hpp"
#include "../XmlIo.hpp"
#include <iostream>
#include <string>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE insert_objects

#include <boost/test/unit_test.hpp>

/**
 * This is a unit test for XmlIo file opening method.
 * If path/name is wrong, exception should be thrown. 
 */

BOOST_AUTO_TEST_CASE(CyclicQueueUnitTest)
{
	XmlIo xml("wrongname.xml\0");
}
