#include <QApplication>
#include "../dialog.hpp"

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	Dialog dialog(nullptr, 43, 2, 12);
	QString path = "/home/konrad/Programming/linux/workspace/evolva/sprites/grass.png";
	for (int i = 0; i < 43; i++) {
		for (int j = 0; j < 2; j++) {

			if (i*j % 2)
				path = QString::fromStdString("/home/konrad/Programming/linux/workspace/evolva/sprites/grass.png");
			else
				path = QString::fromStdString("/home/konrad/Programming/linux/workspace/evolva/sprites/soil.png");
			
			dialog.CreateSurfaceObject(path, i, j);
		}
	}	
	dialog.show();
	for (int i = 0; i < 43; i++) {
		for (int j = 0; j < 2; j++) {

			if (!(i*j % 2))
				path = QString::fromStdString("/home/konrad/Programming/linux/workspace/evolva/sprites/grass.png");
			else
				path = QString::fromStdString("/home/konrad/Programming/linux/workspace/evolva/sprites/soil.png");
			
			dialog.RemoveSurfaceObject(i, j);
			dialog.CreateSurfaceObject(path, i, j);
		}
	}
	a.exec();
	return 0;
}
