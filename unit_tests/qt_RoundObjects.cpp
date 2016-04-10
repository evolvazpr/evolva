#include "dialog.hpp"
#include <QApplication>
#include <QThread>
#include <QtConcurrentRun>

class Sleeper : public QThread
{
public:
    static void usleep(unsigned long usecs){QThread::usleep(usecs);}
    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
    static void sleep(unsigned long secs){QThread::sleep(secs);}
};

void test_thread(Dialog* w) {
	    w->removeObject(6);
	    while(1) {
		    w->moveObject(0, 8, 4);
		    Sleeper::sleep(1);
		    w->moveObjectTo(0, 20, 25);
		    Sleeper::sleep(1);
		    w->moveObject(0, -3, -2);
		    Sleeper::sleep(1);
		    w->moveObjectTo(0, 5, 10);
		    Sleeper::sleep(1);
		    w->moveObjectTo(0, 0, 0);
		    Sleeper::sleep(1);
	    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();
    w.createObject(0, 0, 0);
    w.createObject(1, 0, 1);
    w.createObject(2, 2, 2);
    w.createObject(4, 0, 1);
    w.createObject(5, 1, 0);
    w.createObject(6, 30, 30);
    QtConcurrent::run(test_thread, &w);
    return a.exec();
}
