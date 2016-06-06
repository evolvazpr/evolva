#ifndef _DIALOGPROXXY_H_
#define _DIALOGPROXXY_H_

/*

#include <string>

class Dialog;

class DialogProxy {
	private:
	Dialog* dialog_;
	public:
		DialogProxy();
		DialogProxy& operator <<(const std::string text);
		DialogProxy& operator <<(const char* text);
};

extern DialogProxy log;

/**/

#define logger std::cout

#endif
