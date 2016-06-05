#include "GuiProxy.hpp"
#include "gui.hpp"

GuiProxy::DialogProxy() {
	gui_ = Gui::GetInstance();
}

GuiProxy& DialogProxy::operator <<(const std::string text){
	*gui_ << text;
	return *this;
}

GuiProxy& DialogProxy::operator <<(const char* text) {
	*gui_ << text;
	return *this;
}
