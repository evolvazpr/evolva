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

#include "../Gui.hpp"

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	Gui* dialog = Gui::GetInstance(nullptr, 10, 10);
	field = Field::GetInstance(10, 10);
	std::shared_ptr<Flesh> u[2];
	u[0] = std::make_shared<Flesh>(0.3);
	u[1] = std::make_shared<Flesh>(0.4);
	field->InsertNmo(u[0], 2, 2);
	field->InsertNmo(u[1], 3, 3);
	dialog->show();
	a.exec();
	return 0;
}
