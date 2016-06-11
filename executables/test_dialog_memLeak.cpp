#include <QApplication>
#include "../dialog.hpp"

/**
 * @brief manual test for memory leak.
 * 
 * There is a problem with Qt and Valgrind. It generates false positive.
 * To be 100% sure that there is no memory leak, which hugly may occure in
 * dialog class, run this program and see in "top" whether memory consumption grows up.
 *
 * In SpriteObject class it can not occure (no memory allocations).
 */

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);
	Dialog dialog(nullptr, 40, 40, 50);
	QPixmap grass("../sprites/grass.png");
	grass = grass.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	QPixmap soil("../sprites/soil.png");
	soil = soil.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	QPixmap herbivore("../sprites/herbivore.png");
	herbivore = herbivore.scaled(50 * 4, 50 * 4, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	QPixmap carnivore("../sprites/carnivore.png");
	carnivore = carnivore.scaled(50 * 4, 50 * 4, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	while(1) {
		size_t id = 0;
		id = 0;	
		for (int i = 0; i < 40; i++) {
			for (int j = 0; j < 20; j++) {
				dialog.CreateSurfaceObject(grass, i, j);
				dialog.CreateObject(id, herbivore, 4, i, j);
				id++;
			}
		}
		
		for (int i = 0; i < 40; i++) {
			for (int j = 20; j < 40; j++) {
				dialog.CreateSurfaceObject(soil, i, j);
				dialog.CreateObject(id, carnivore, 4, i, j);
				id++;
			}
		}
		id = 0;	
		for (int i = 0; i < 40; i++) {
			for (int j = 0; j < 40; j++) {
				dialog.RemoveObject(id);
				dialog.RemoveSurfaceObject(i, j);
				id++;
			}	
		}
	}
}
