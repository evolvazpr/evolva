#include "Pixmap.hpp"

Pixmap::Pixmap(const uint pixels_per_object) {
		pixels_per_object_ = pixels_per_object;
}
	
Pixmap::Pixmap(const Pixmap & p) {
		pixels_per_object_ = p.pixels_per_object_;
		pixmap_ = p.pixmap_;
		sprite_cnt_ = p.sprite_cnt_;
}

Pixmap::Pixmap(Pixmap&& p) {
		pixels_per_object_ = p.pixels_per_object_;
		pixmap_ = p.pixmap_;
		sprite_cnt_ = p.sprite_cnt_;
}

void Pixmap::SetObject(QString path, uint sprite_cnt) {
		pixmap_ = QPixmap(path);
		sprite_cnt_ = sprite_cnt;
		pixmap_ = pixmap_.scaled(pixels_per_object_ * sprite_cnt_, pixels_per_object_ * sprite_cnt_, 
					 Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

const QPixmap& Pixmap::GetPixmap() const { return pixmap_; }
uint Pixmap::GetSpriteCnt() const { return sprite_cnt_; }
