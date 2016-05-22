#include <string>
#include "../EvolvaException.hpp"
#include "../XmlIo.hpp"
#include <iostream>
#include <string>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE insert_objects

#include <boost/test/unit_test.hpp>

/**
 * This is a unit test for XmlIo reading strings.
 * And it shows some problem with templates.
 *
 * Wider description of problem is at 
 * http://stackoverflow.com/questions/7741531/conversion-operator-template-specialization.
 * TODO: do something with it. Somehow.
 */

BOOST_AUTO_TEST_CASE(CyclicQueueUnitTest)
{
	XmlIo xml("gui.xml\0");
	std::string out1 = xml["grass"];
	BOOST_CHECK(out1 == "/home/konrad/Programming/linux/workspace/evolva/sprites/grass.png");
	std::string out2 = xml["sand"];
	BOOST_CHECK(out2 == "/home/konrad/Programming/linux/workspace/evolva/sprites/sand.png");	
	std::string out3 = xml["water"];
	BOOST_CHECK(out3 == "/home/konrad/Programming/linux/workspace/evolva/sprites/water.png");
	std::string out4 = xml["soil"];
	BOOST_CHECK(out4 == "/home/konrad/Programming/linux/workspace/evolva/sprites/soil.png");
} 
