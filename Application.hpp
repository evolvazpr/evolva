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
	QMutex *mutex_;
	static Logic* instance_;
	Logic(QMutex *mutex);
	QString GetObjectType(std::shared_ptr<const CellObject> object);
	QString GetGroundType(FieldCell::Ground type);
public:
	void Init();
	static Logic *GetInstance(QMutex *mutex = nullptr);
	void CreateObject(std::shared_ptr<const CellObject> object, const int x, const int y);
	void CreateSurfaceObject(const FieldCell::Ground ground_type, const int x, const int y);
	void RemoveSurfaceObject(const int x, const int y);
	void MoveObject(std::shared_ptr<const CellObject> object, const int x, const int y);
	void MoveObjectTo(std::shared_ptr<const CellObject> object, const int x, const int y);
	void RemoveObject(std::shared_ptr<const CellObject> object);
	
public slots:
	void LogicIteration();

signals:
	void CreateObject_s(const uint id, const QString type, const int x, const int y);
	void CreateSurfaceObject_s(const QString type, const int x, const int y);
	void RemoveSurfaceObject_s(const int x, const int y);
	void MoveObject_s(const uint id, const int x, const int y);
	void MoveObjectTo_s(const uint id, const int x, const int y);
	void RemoveObject_s(const uint id);
};

class Application : public QApplication {
	Q_OBJECT
private:
	QMutex mutex_;
	Logic* logic_;
	Dialog* dialog_;
	QThread logic_thread_;
	XmlIo gui_settings_;
	void ConnectSignals();
public:
	Application(int& argc, char **argv);
	void Init();

public slots:
	void CreateObject(const uint id, const QString type, const int x, const int y);
	void CreateSurfaceObject(const QString type, const int x, const int y);
	void ClearMutex();
};

#endif 

