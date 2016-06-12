#ifndef _PIXMAP_HPP_
#define _PIXMAP_HPP_
#include <QPixmap>

/**
 * @brief Prototype class
 *
 * Object of this class contains all sprites used by GUI.
 * It was introduced to improve speed of GUI. Without it, every
 * graphics object creation needed to load sprite from external file.
 */
class Pixmap {
private:
	uint pixels_per_object_;
	QPixmap pixmap_;
	uint sprite_cnt_;
public:
	Pixmap(const uint pixels_per_object);
	Pixmap(const Pixmap & p);
	Pixmap(Pixmap&& p);

	void SetObject(QString path, uint sprite_cnt);
	const QPixmap& GetPixmap() const;
	uint GetSpriteCnt() const;
};

#endif
