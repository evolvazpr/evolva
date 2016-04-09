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
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();
    w.createObject(0, 0, 0);
    w.createObject(1, 1, 1);
    w.createObject(2, 2, 2);
    w.createObject(3, 3, 3);
    w.createObject(4, 0, 1);
    w.createObject(5, 1, 0);
    w.createObject(6, 30, 30);
    QtConcurrent::run(test_thread, &w);
    return a.exec();
}
