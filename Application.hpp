#ifndef _APPLICATION_H_
#define _APPLICATION_H_
#include <QApplication>
#include <QMessageBox>
#include <boost/format.hpp>
#include <boost/tokenizer.hpp>

#include "Field.hpp"
#include "CellObject.hpp"
#include "DnaGenerator.hpp"
#include "Unit.hpp"
#include "CyclicQueue.hpp"
#include "dialog.hpp"
#include "XmlIo.hpp"
#include "Pixmap.hpp"

/** @brief Class which connects GUI and logic
 */
class Application : public QApplication {
	Q_OBJECT
private:
	static Application* instance_;

	std::shared_ptr<Field> field_;
	std::unordered_map<std::string, Pixmap> pixmaps_;

	XmlIo gui_settings_;
	XmlIo logic_settings_;
	Dialog dialog_;
	void ConnectSignals();

	QString GetObjectType(std::shared_ptr<const CellObject> object) const;
	QString GetGroundType(FieldCell::Ground type) const;
	const QPixmap& GetObjectPixmap(std::shared_ptr<const CellObject> object, uint* sprite_cnt) const;
	const QPixmap& GetObjectPixmap(const FieldCell::Ground ground_type) const;
	boost::format CreateStatistics(const int x, const int y) const;
	void PrepareInitialObjects(CellObject::Type type);	
	void PrepareInitialPlants();
	Application(int& argc, char **argv);
	void LogicInit();
	void PixmapContainerInit();
	void AddToPixmaps(std::string name);

public:	
	~Application();
	void Init(bool logic);

	void CreateObject(std::shared_ptr<const CellObject> object, const int x, const int y);
	void CreateSurfaceObject(const FieldCell::Ground ground_type, const int x, const int y);
	void ReplaceSurfaceObject(const FieldCell::Ground ground_type, const int x, const int y);
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
	void MoveLogicToEndOfRound(uint rounds);
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

