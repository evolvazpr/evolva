#ifndef _DIALOGPROXXY_H_
#define _DIALOGPROXXY_H_
#include <string>

class Gui;

class GuiProxy {
	private:
	Gui* gui_;
	public:
		GuiProxy();
		GuiProxy& operator <<(const std::string text);
		GuiProxy& operator <<(const char* text);
};

#endif
