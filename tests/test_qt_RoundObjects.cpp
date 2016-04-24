#include "../dialog.hpp"
#include <QApplication>
#include <QThread>
#include <QtConcurrentRun>
#include <QColor> 
#include <iostream>

class Sleeper : public QThread
{
public:
    static void usleep(unsigned long usecs){QThread::usleep(usecs);}
    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
    static void sleep(unsigned long secs){QThread::sleep(secs);}
};

QAtomicInt run_thread(1);

void test_thread(Dialog* w) {
	    while(run_thread.fetchAndAddAcquire(0)) {
		    w->moveObjectTo(0, (size_t)1, (size_t)0);
		    Sleeper::sleep(2);
		    w->moveObjectTo(0, (size_t)2, (size_t)0);
		    Sleeper::sleep(2);
 		    w->moveObjectTo(0, (size_t)3, (size_t)0);
		    Sleeper::sleep(2);
		    w->moveObjectTo(0, (size_t)3, (size_t)1);
		    Sleeper::sleep(2);
		    w->moveObjectTo(0, (size_t)3, (size_t)2);
		    Sleeper::sleep(2);
 		    w->moveObjectTo(0, (size_t)3, (size_t)3);
		    Sleeper::sleep(2);
		    w->moveObjectTo(0, (size_t)9, (size_t)9);
		    Sleeper::sleep(2);
	    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();
    w.createObject(0, 0, 0, Qt::red);
    w.createObject(1, 0, 1, Qt::blue);
    w.createObject(2, 1, 1, Qt::black);
    //QFuture<void> future = QtConcurrent::run(test_thread, &w);
    a.exec();
    run_thread.deref();
    //future.waitForFinished();
    return 0;
}
