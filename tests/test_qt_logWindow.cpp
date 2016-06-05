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
	Dialog* dialog = Dialog::GetInstance(nullptr, 10, 10);
	dialog->show();
	dialog->AppendTextToLog("test_1");
	dialog->AppendTextToLog("test_2");
	dialog->AppendTextToLog("test_3");
	a.exec();
	return 0;
}
