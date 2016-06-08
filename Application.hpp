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



/** @brief
 * proxy between logic core (which is in second thread) and GUI (first/main thread, because
 * this is how qt works if I have understood documentation properly).
 */
class Application : public QApplication {
	Q_OBJECT
private:
	static Application* instance_;

	std::shared_ptr<Field> field_;
	Dialog dialog_;
	XmlIo gui_settings_;
	void ConnectSignals();

	QString GetObjectType(std::shared_ptr<const CellObject> object);
	QString GetGroundType(FieldCell::Ground type);
	boost::format CreateStatistics(const int x, const int y);
	
	Application(int& argc, char **argv);
	void LogicInit();

public:	
	~Application();
	void Init();

	void CreateObject(std::shared_ptr<const CellObject> object, const int x, const int y);
	void CreateSurfaceObject(const FieldCell::Ground ground_type, const int x, const int y);
	void RemoveSurfaceObject(const int x, const int y);
	void MoveObject(std::shared_ptr<const CellObject> object, const int x, const int y);
	void MoveObjectTo(std::shared_ptr<const CellObject> object, const int x, const int y);
	void RemoveObject(std::shared_ptr<const CellObject> object);
	void UpdateLog(const QString text);
	static Application* GetInstance(int argc = 0, char **argv = nullptr);
	template <class T> Application& operator <<(const T object);

public slots:
	void SpriteObjectClicked(int x, int y);
	void LogicIteration();
};

/**
 * @brief stream operator overload.
 * Used to append text to log window.
 * @param s - std::string to append.
 */
template <class T> Application& Application::operator <<(const T object) {
	std::stringstream ss;
	ss << object;
	std::string text = ss.str();
	UpdateLog(QString::fromStdString(text));
	return *this;
}
#endif 

