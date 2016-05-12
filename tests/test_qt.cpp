#include <QApplication>
#include "../dialog.hpp"
#include <QColor>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Dialog w;
	w.show();
	//w.CreateObject(0, 0, 0);
	//w.CreateObject(1, 1, 1);
	//w.CreateFloorObject(Qt::red, 5, 5);
	//w.CreateFloorObject(Qt::red, 7, 7);
	//w.RemoveFloorObject(7, 7);
	//w.CreateObject(3, 3, 3);	
	a.exec();
	return 0;
}
