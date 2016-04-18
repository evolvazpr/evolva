#include "../dialog.hpp"
#include <QApplication>
#include <QThread>
#include <QtConcurrentRun>
#include <QColor> 

class Sleeper : public QThread
{
public:
    static void usleep(unsigned long usecs){QThread::usleep(usecs);}
    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
    static void sleep(unsigned long secs){QThread::sleep(secs);}
};

QAtomicInt run_thread(1);

void test_thread(Dialog* w) {
	    w->removeObject(6);
	    while(run_thread.fetchAndAddAcquire(0)) {
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
    w.createObject(0, 0, 0, Qt::red);
    w.createObject(1, 0, 1, Qt::blue);
    w.createObject(2, 1, 0, Qt::yellow);
    w.createObject(4, 0, 2, Qt::black);
    w.createObject(5, 2, 0, Qt::green);
    w.createObject(6, 30, 30, Qt::green);
    QFuture<void> future = QtConcurrent::run(test_thread, &w);
    a.exec();
    run_thread.deref();
    future.waitForFinished();
    return 0;
}
