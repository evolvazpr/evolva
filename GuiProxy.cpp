#include "GuiProxy.hpp"
#include "Gui.hpp"

GuiProxy::GuiProxy() {
	gui_ = Gui::GetInstance();
}

GuiProxy& GuiProxy::operator <<(const std::string text){
	*gui_ << text;
	return *this;
}

GuiProxy& GuiProxy::operator <<(const char* text) {
	*gui_ << text;
	return *this;
}
