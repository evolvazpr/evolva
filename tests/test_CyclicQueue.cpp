#include "../CyclicQueue.hpp"
#include "../CellObject.hpp"
#include "../Field.hpp"
#define private public
#define protected public
#include "../DnaGenerator.hpp"
#include "../Unit.hpp"
#undef protected
#undef private
#include "../dialog.hpp"
#include <QApplication>


#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE insert_objects

#include <boost/test/unit_test.hpp>

/**
 * This is a unit test for CyclicQueue
 */

BOOST_AUTO_TEST_CASE(CyclicQueueUnitTest)
{
	int argc = 1;
	char *argv[2];
	argv[0] = (char *)"insert"; //only for QApplication creation
	QApplication a(argc, argv);
	Dialog::GetInstance();

	field = Field::GetInstance(10, 10); 
	CyclicQueue queue;
	std::shared_ptr<DnaCode> dna_ptr = std::make_shared<DnaCode>();


	DnaGenerator gen(dna_ptr);

	std::shared_ptr<Unit> u[2];
	u[0] = std::make_shared<Unit>(gen.Generate());
	u[1] = std::make_shared<Unit>(gen.Generate());
	queue.Begin();
	queue.Insortion(u[0]);
	queue.Insortion(u[1]);
	/*TODO: Don't know how to test it - right now all 
	 * movable objects have MovePriority parameter which 
	 * equals to 0.
	 */
	queue.Begin();
	BOOST_CHECK(queue.Next() == false);
	BOOST_CHECK(queue.IsEnd() == false);
	BOOST_CHECK(queue.Get() == u[0]);
	BOOST_CHECK(queue.Next() == true);
	BOOST_CHECK(queue.IsEnd() == true);
	BOOST_CHECK(queue.Get() == u[1]);
	BOOST_CHECK(queue.Next() == false);
	BOOST_CHECK(queue.IsEnd() == false);
	BOOST_CHECK(queue.Get() == u[0]);	
} 
