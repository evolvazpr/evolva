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
	Gui* gui = Gui::GetInstance(nullptr, 10, 10);
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			gui->CreateSurfaceObject(FieldCell::Ground::GROUND, i, j);			
		}
	}

	for (int i = 4; i < 7; i++) {
		for (int j = 4; j < 7; j++) {
			gui->RemoveSurfaceObject(i, j);
			gui->CreateSurfaceObject(FieldCell::Ground::GRASS, i, j);
		}
	}

	gui->show();
	a.exec();
	return 0;
}
