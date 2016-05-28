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

void EvolvaInit(Dialog* w) {
	field = Field::GetInstance(10, 10, w);
	std::shared_ptr<DnaCode> dna_ptr = std::make_shared<DnaCode>();

	DnaGenerator gen(dna_ptr);
	gen.variability_ = 30.0;

	std::shared_ptr<Unit> u[2];
	u[0] = std::make_shared<Unit>(gen.Generate());
	u[1] = std::make_shared<Unit>(gen.Generate());

	u[0]->energy_ = 200.0;
	u[0]->death_ = 10000;
	u[0]->dna_["normal_weight"] = 220.0;
	field->InsertObject(u[0], 6, 6);
	u[1]->energy_ = 200.0;
	u[1]->death_ = 10000;
	u[1]->dna_["normal_weight"] = 200.0;
	field->InsertObject(u[1], 7, 7);

	field->InsertNmo(std::make_shared<Tree>(50.0), 0, 0);
	field->InsertNmo(std::make_shared<Tree>(80.0), 0, 9);
	field->InsertNmo(std::make_shared<Tree>(20.0), 9, 0);
	field->InsertNmo(std::make_shared<Tree>(24.0), 9, 9);
	field->InsertNmo(std::make_shared<Tree>(500.0), 1, 0);

	w->RemoveSurfaceObject(9, 9);
	w->RemoveSurfaceObject(0, 0);
	w->RemoveSurfaceObject(1, 5);
	w->CreateSurfaceObject(Dialog::Surface::SAND, 9, 9);
	w->RemoveSurfaceObject(9, 9);
	field->BeginCycle();
	field->Play();
	Unit *xz = u[0].get();
	xz->IsRemoved();

}

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	Dialog w;
	EvolvaInit(&w);
	w.show();
	a.exec();
	return 0;
}
