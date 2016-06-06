#include "Application.hpp"
#include <iostream>
#include <QMetaType>

Application::Application(int& argc, char **argv) : QApplication(argc, argv), gui_settings_("gui.xml") {

}

Logic *Logic::instance_ = nullptr;

Logic *Logic::GetInstance(QMutex *mutex) {
	if (instance_ == nullptr) {
		instance_ = new Logic(mutex);
	}
	return instance_;
}

Logic::Logic(QMutex *mutex) : QObject(), mutex_(mutex) {}


void Logic::LogicIteration() {
	Tui tui;
	field_->Next();
	tui.PrintField();
}

void Logic::CreateObject(std::shared_ptr<const CellObject> object, const int x, const int y) {
	mutex_->lock();
	emit SignalGui(object->GetId(), GetObjectType(object), x, y, GuiHandler::CREATEOBJECT);
}
void Logic::CreateSurfaceObject(const FieldCell::Ground ground_type, const int x, const int y) {
	emit SignalGui(0, GetGroundType(ground_type), x, y, GuiHandler::CREATESURFACEOBJECT);
}
void Logic::RemoveSurfaceObject(const int x, const int y) {
	emit SignalGui(0, QString(""), x, y, GuiHandler::REMOVESURFACEOBJECT);
}
void Logic::MoveObject(std::shared_ptr<const CellObject> object, const int x, const int y) {
	mutex_->lock();
	emit SignalGui(object->GetId(),QString(""), x, y, GuiHandler::MOVEOBJECT);
}
void Logic::MoveObjectTo(std::shared_ptr<const CellObject> object, const int x, const int y) {
	mutex_->lock();
	emit SignalGui(object->GetId(), QString(""), x, y, GuiHandler::MOVEOBJECTTO);
}

void Logic::RemoveObject(std::shared_ptr<const CellObject> object) {
	mutex_->lock();
	emit SignalGui(object->GetId(), QString(""), 0, 0, GuiHandler::REMOVEOBJECT);
}

void Logic::Init() {
	field_ = Field::GetInstance(30, 30);
	{
		std::shared_ptr<DnaCode> dna_ptr = std::make_shared<DnaCode>();
		DnaCode &dna = *dna_ptr;
		dna["intelligence"] = 80.0;
		dna["agility"] = 30.0;
		dna["speed"] = 80.0;
		dna["strength"] = 20.4;
		dna["death_time_u"] = 50.0;
		dna["death_time_s"] = 2.68;
		dna["mutability"] = 10.0;
		dna["mutation_stability"] = 1.1;
		dna["fatigue_death_threshold"] = 80.0;
		dna["health_death_threshold"] = 20.0;
		dna["fatigue_regeneration"] = 10.0;
		dna["health_regeneration_sleep"] = 5.0;
		dna["health_regeneration"] = 0.0;
		dna["requirements.normal_turn"] = 10.0;
		dna["requirements.sleep_turn"] = 10.0;
		dna["requirements.eating"] = 2.0;
		dna["requirements.mating"] = 25.0;
		dna["requirements.step"] = 2.0;
		dna["requirements.attack"] = 50.0;
		dna["requirements.childbirth"] = 40.0;
		dna["wakeup_threshold"] = 20.0;
		dna["fatigue_turn_increase"] = 2.0;
		dna["normal_weight"] = 80.0;
		dna["pregnancy_time"] = 2.0;
		dna["herbivore"] = 100.0;
		dna["carnivore"] = 0.0;
		dna["vision"] = 15.0;

		DnaGenerator gen(dna_ptr);
		gen.variability_ = 30.0;

		std::shared_ptr<Unit> u[10];
		u[0] = std::make_shared<Unit>(gen.Generate());
		u[1] = std::make_shared<Unit>(gen.Generate());
		u[2] = std::make_shared<Unit>(gen.Generate());
		u[3] = std::make_shared<Unit>(gen.Generate());
		u[4] = std::make_shared<Unit>(gen.Generate());
		u[5] = std::make_shared<Unit>(gen.Generate());
		u[6] = std::make_shared<Unit>(gen.Generate());
		u[7] = std::make_shared<Unit>(gen.Generate());
		u[8] = std::make_shared<Unit>(gen.Generate());
		u[9] = std::make_shared<Unit>(gen.Generate());

		field->InsertObject(u[0], 0, 0);

		field->InsertObject(u[1], 9, 9);
		field->InsertObject(u[2], 7, 0);
		field->InsertObject(u[3], 11, 1);
		field->InsertObject(u[4], 10, 2);
		field->InsertObject(u[5], 9, 3);
		field->InsertObject(u[6], 2, 0);
		field->InsertObject(u[7], 22, 2);
		field->InsertObject(u[8], 24, 0);

	}
	{

		std::shared_ptr<DnaCode> dna_ptr = std::make_shared<DnaCode>();
		DnaCode &dna = *dna_ptr;
		dna["intelligence"] = 50.0;
		dna["agility"] = 60.0;
		dna["speed"] = 80.0;
		dna["strength"] = 70.4;
		dna["death_time_u"] = 60.0;
		dna["death_time_s"] = 1.5;
		dna["mutability"] = 20.0;
		dna["mutation_stability"] = 1.1;
		dna["fatigue_death_threshold"] = 95.0;
		dna["health_death_threshold"] = 5.0;
		dna["fatigue_regeneration"] = 15.0;
		dna["health_regeneration_sleep"] = 10.0;
		dna["health_regeneration"] = 0.0;
		dna["requirements.normal_turn"] = 12.0;
		dna["requirements.sleep_turn"] = 1.0;
		dna["requirements.eating"] = 5.0;
		dna["requirements.mating"] = 20.0;
		dna["requirements.step"] = 1.0;
		dna["requirements.attack"] = 35.0;
		dna["requirements.childbirth"] = 60.0;
		dna["wakeup_threshold"] = 20.0;
		dna["fatigue_turn_increase"] = 4.0;
		dna["normal_weight"] = 800.0;
		dna["pregnancy_time"] = 3.0;
		dna["herbivore"] = 0.0;
		dna["carnivore"] = 100.0;
		dna["vision"] = 50.0;

		DnaGenerator gen(dna_ptr);
		gen.variability_ = 30.0;

		std::shared_ptr<Unit> u[6];
		u[0] = std::make_shared<Unit>(gen.Generate());
		u[1] = std::make_shared<Unit>(gen.Generate());
		u[2] = std::make_shared<Unit>(gen.Generate());
		u[3] = std::make_shared<Unit>(gen.Generate());
		u[4] = std::make_shared<Unit>(gen.Generate());
		u[5] = std::make_shared<Unit>(gen.Generate());

		field->InsertObject(u[0], 19, 19);

		field->InsertObject(u[1], 0, 15);
		field->InsertObject(u[2], 1, 15);
		field->InsertObject(u[3], 2, 15);
		field->InsertObject(u[4], 1, 18);
		field->InsertObject(u[5], 2, 18);

	}

	field->InsertNmo(std::make_shared<Tree>(50.0), 1, 1);
	field->InsertNmo(std::make_shared<Tree>(80.0), 7, 2);
	field->InsertNmo(std::make_shared<Tree>(20.0), 3, 4);
	field->InsertNmo(std::make_shared<Tree>(24.0), 4, 4);
	field->InsertNmo(std::make_shared<Tree>(500.0), 2, 8);
	field->InsertNmo(std::make_shared<Tree>(100.0), 0, 9);

	for (int i = 0; i < 10; ++i) field->GrowPlants();
	field->BeginCycle();
	field->Play();
}

void Application::Init() {
	qRegisterMetaType<GuiHandler>("GuiHandler");
	dialog_ = Dialog::GetInstance(nullptr, 30, 30);
	dialog_->show();
	logic_ = Logic::GetInstance(&mutex_);
	ConnectSignals();
	logic_->moveToThread(&logic_thread_);
	logic_thread_.start(QThread::TimeCriticalPriority);
	logic_->Init();	
}

QString Logic::GetObjectType(std::shared_ptr<const CellObject> object) {
	QString obj_type;
	if (object->GetType(CellObject::Type::CARNIVORE)) { 
		if (object->GetType(CellObject::Type::FLESH))
			obj_type = "carnivore_dead";
		else
			obj_type = "carnivore";
	} else if (object->GetType(CellObject::Type::HERBIVORE)) {
		if (object->GetType(CellObject::Type::FLESH))
			obj_type = "herbivore_dead";
		else
			obj_type = "herbivore";
	} else if (object->GetType(CellObject::Type::PLANT)) {
		obj_type = "tree";
	} else {
		obj_type = "stone";
	}	
	return obj_type;
}

QString Logic::GetGroundType(FieldCell::Ground type) {
	QString surface;
	if (type == FieldCell::Ground::GRASS)
		surface = "grass";
	else
		surface = "soil";
	return surface;
}

void Application::CreateObject(const uint id, const QString type, const int x, const int y) {
	std::string type_s = type.toStdString();
	QString path = QString::fromStdString(gui_settings_[type_s]["path"]);
	int sprite_cnt = gui_settings_[type_s]["sprite_cnt"];
	dialog_->CreateObject(id, path, sprite_cnt, x, y);	
	ClearMutex();
}

void Application::CreateSurfaceObject(const QString type, const int x, const int y) {
	std::string type_s = type.toStdString();
	QString path = QString::fromStdString(gui_settings_[type_s]["path"]);
	dialog_->CreateSurfaceObject(path, x, y);
	ClearMutex();
}

void Application::RemoveSurfaceObject(const int x, const int y) {
	dialog_->RemoveSurfaceObject(x, y);
	ClearMutex();
}

void Application::MoveObject(const uint id, const int x, const int y) {
	dialog_->MoveObject(id, x, y);
}

void Application::MoveObjectTo(const uint id, const int x, const int y) {
	dialog_->MoveObjectTo(id, x, y);
}

void Application::RemoveObject(const uint id) {
	dialog_->RemoveObject(id);
	ClearMutex();		
}

void Application::ConnectSignals() {
	connect(dialog_, SIGNAL(NextLogicIteration()), logic_, SLOT(LogicIteration()));
	connect(logic_, SIGNAL(SignalGui(const uint, const QString, const int, const int, 
		GuiHandler)), this, SLOT(GuiSlot(const uint, const QString,
		const int, const int, GuiHandler)));
	connect(dialog_, SIGNAL(ClearMutex()), this, SLOT(ClearMutex()));
	connect(dialog_, SIGNAL(SpriteObjectClicked(int , int)), this, SLOT(SpriteObjectClicked(int , int)));
}

void Application::GuiSlot(const uint id, const QString type, const int x, const int y, 
			  GuiHandler command) {
	switch (command) {
	case GuiHandler::CREATEOBJECT:
		CreateObject(id, type, x, y);
		break;
	case GuiHandler::CREATESURFACEOBJECT:
		CreateSurfaceObject(type, x, y);
		break;
	case GuiHandler::REMOVESURFACEOBJECT:
		RemoveSurfaceObject(x, y);
		break;
	case GuiHandler::MOVEOBJECT:
		MoveObject(id, x, y);
		break;
	case GuiHandler::MOVEOBJECTTO:
		MoveObjectTo(id, x, y);
		break;
	case GuiHandler::REMOVEOBJECT:
		RemoveObject(id);
		break;
	default:
		break;
	}
}

void Application::ClearMutex() {
	mutex_.unlock();
}

/**
 * @brief method used when sprite object was clicked (so stats window will be updated).
 * This method creates content of stats window.
 * param cell - Field's cell in which object, represented by clicked sprite, resides.
 */
boost::format Logic::CreateStatistics(const int x, const int y) {
	std::shared_ptr<FieldCell> cell = field_->GetCell(x , y);
	std::shared_ptr<CellObject> object = cell->GetObject();
	std::shared_ptr<Unit> unit;
	std::shared_ptr<Plant> plant;
	boost::format form;
		
	if (object.use_count() == 0)
		throw EvolvaException("Dialog::CreateStatistics, cell->IsEmpty() failed.");

	form = boost::format("Id: %1%\nx: %2%\ny: %3%\nEnergy: %4%\nFatigue: %5%\nType: %6%");
	form % object->GetId();
	form % object->GetX();
	form % object->GetY();
	
	if (object->GetType(CellObject::Type::CARNIVORE) || object->GetType(CellObject::Type::HERBIVORE))  {
		unit = std::dynamic_pointer_cast<Unit>(object); 
		/* Above line proofs wrong interface of CellObject.
		 * I should write there GetEnergy() and GetFatigue() virtual methods also.
		 * Without it, I must cast.
		 */
		if (unit.use_count() == 0)
			throw EvolvaException("Dialog::CreateStatistics, object->GetType(MOVABLE) failed");
		form % unit->GetEnergy();	
		form % unit->GetFatigue();
	} else {

		if(object->GetType(CellObject::Type::PLANT)) {
			plant = std::dynamic_pointer_cast<Plant>(object);
			if (plant.use_count() == 0)
				throw EvolvaException("Dialog::CreateStatistics, object->GetType(PLANT) failed.");		
			form % plant->GetEnergy();
			form % 0;
		} else {
			form % 0;
			form % 0;
		}	
	}
	form % GetObjectType(object).toStdString();	
	return form;
}

void Application::SpriteObjectClicked(int x, int y) {
	boost::format format = logic_->CreateStatistics(x, y);
	dialog_->UpdateStatistics(QString::fromStdString(format.str()));
}
