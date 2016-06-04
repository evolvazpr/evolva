#include "../Field.hpp"
#include "../CellObject.hpp"
#include "../Tui.hpp"
#define private public
#define protected public
#include "../DnaGenerator.hpp"
#include "../Unit.hpp"
#undef protected
#undef private
#include <iostream>
#include <stdio.h>

#include "../CyclicQueue.hpp"
#include <memory>
#include <QApplication>

#include "../dialog.hpp"

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	Dialog* w = Dialog::GetInstance();
	*w << std::string("first\n");
	*w << std::string("second\n");
	*w << std::string("third");
	*w << "fourth";
	*w << "fifth" << " sixth";
	w->show();
	a.exec();
	return 0;
}
