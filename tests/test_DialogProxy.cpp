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
#include "../DialogProxy.hpp"

void Test() {
}

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	Dialog* w = Dialog::GetInstance();
	DialogProxy proxy;
	proxy << std::string("first\n");
	proxy << std::string("second\n");
	proxy << std::string("third\n");
	proxy << "fourth\n";
	proxy << "fifth " << "sixth";
	w->show();
	a.exec();
	return 0;
}
