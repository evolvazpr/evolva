#ifndef _APPLICATION_H_
#define _APPLICATION_H_
#include <QApplication>
#include <memory>

#include "Field.hpp"
#include "CellObject.hpp"
#include "Tui.hpp"
#define private public
#define protected public
#include "DnaGenerator.hpp"
#include "Unit.hpp"
#undef protected
#undef private
#include "CyclicQueue.hpp"
#include "dialog.hpp"


class Logic : public QObject {
	Q_OBJECT
	private:
		std::shared_ptr<Field> field_;
	public:
		Logic();
		void Init();
	public slots:
		void LogicIteration();
};

class Application : public QApplication {
	Q_OBJECT
	private:
		Logic logic_;
		Dialog* dialog_;
		QMutex mutex_;
		QThread logic_thread_;
	public:
		Application(int& argc, char **argv);
		void Init();
};

#endif 

