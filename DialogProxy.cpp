#include "DialogProxy.hpp"
#include "dialog.hpp"

DialogProxy::DialogProxy() {
	dialog_ = Dialog::GetInstance();
}

DialogProxy& DialogProxy::operator <<(const std::string text){
	*dialog_ << text;
	return *this;
}

DialogProxy& DialogProxy::operator <<(const char* text) {
	*dialog_ << text;
	return *this;
}
