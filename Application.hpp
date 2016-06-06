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
#include <boost/format.hpp>
#include <QThread>
#include "XmlIo.hpp"

class Logic;

enum class GuiHandler : uint {
	CREATEOBJECT,
	CREATESURFACEOBJECT,
	REMOVESURFACEOBJECT,
	MOVEOBJECT,
	MOVEOBJECTTO,
	REMOVEOBJECT,
	WRITETEXT
};

Q_DECLARE_METATYPE(GuiHandler)

/** @brief
 * proxy between logic core (which is in second thread) and GUI (first/main thread, because
 * this is how qt works if I have understood documentation properly).
 */
class Application : public QApplication {
	Q_OBJECT
private:
	QMutex mutex_;
	Logic* logic_;
	Dialog* dialog_;
	QThread logic_thread_;
	XmlIo gui_settings_;
	void ConnectSignals();
	void CreateObject(const uint id, const QString type, const int x, const int y);
	void CreateSurfaceObject(const QString type, const int x, const int y);
	void RemoveSurfaceObject(const int x, const int y);
	void MoveObject(const uint id, const int x, const int y);
	void MoveObjectTo(const uint id, const int x, const int y);
	void RemoveObject(const uint id);
	void UpdateLog(const QString text);
public:
	~Application();
	Application(int& argc, char **argv);
	void Init();

public slots:
	void ClearMutex();
	void GuiSlot(const uint id, const QString type, const int x, const int y, GuiHandler command);
	void SpriteObjectClicked(int x, int y);
	void OnExit();
};


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
	void WriteToLog(const QString data);
	boost::format CreateStatistics(const int x, const int y);
	template <class T> Logic& operator <<(const T object);

public slots:
	void LogicIteration();

signals:
	void SignalGui(const uint id, const QString type, const int x, const int y, GuiHandler command);
};

/**
 * @brief stream operator overload.
 * Used to append text to log window.
 * @param s - std::string to append.
 */
template <class T> Logic& Logic::operator <<(const T object) {
	std::stringstream ss;
	ss << object;
	std::string text = ss.str();
	emit SignalGui(0, QString::fromStdString(text), 0, 0, GuiHandler::WRITETEXT);
	return *this;
}
/**
 * @brief stream operator overload.
 * Used to append text to log window.
 * @param s - std::string to append.
 */
/*template <> Logic& Logic::operator <<(const std::string s) {	
	emit SignalGui(0, QString::fromStdString(s), 0, 0, GuiHandler::WRITETEXT);
	return *this;
}
*/
/**
 * @brief stream operator overload.
 * Used to append text to log window.
 * @param s - char pointer to append.
 */
/*template <> Logic& Logic::operator <<(const char* s) {	
	emit SignalGui(0, QString(s), 0, 0, GuiHandler::WRITETEXT);
	return *this;
}
*/
#endif 

