#include "Application.hpp"
#include <iostream>
#include <QMetaType>

Application::Application(int& argc, char **argv) : QApplication(argc, argv), dialog_(nullptr, 30, 30), gui_settings_("gui.xml") {

}

Application* Application::instance_ = nullptr;

Application* Application::GetInstance(int argc, char **argv) {
	
	struct make_shared_enabler : public Application {
		public:
			make_shared_enabler(int& argc, char**argv) : Application(argc, argv){}
	};

	if (instance_ == nullptr) {
		instance_ = new Application(argc, argv);
	}
	return instance_;
}

void Application::LogicIteration() {
	Tui tui;
	if(!field_->Next()) {
		exit();
	}
	tui.PrintField();
}

void Application::CreateObject(std::shared_ptr<const CellObject> object, const int x, const int y) {
	std::string type_s = GetObjectType(object).toStdString();
	QString path = QString::fromStdString(gui_settings_[type_s]["path"]);
	int sprite_cnt = gui_settings_[type_s]["sprite_cnt"];
	dialog_.CreateObject(object->GetId(), path, sprite_cnt, x, y);
}
void Application::CreateSurfaceObject(const FieldCell::Ground ground_type, const int x, const int y) {
	std::string type_s = GetGroundType(ground_type).toStdString();
	QString path = QString::fromStdString(gui_settings_[type_s]["path"]);
	dialog_.CreateSurfaceObject(path, x, y);
}

void Application::MoveObject(std::shared_ptr<const CellObject> object, const int x, const int y) {
	dialog_.MoveObject(object->GetId(), x, y);
}
void Application::MoveObjectTo(std::shared_ptr<const CellObject> object, const int x, const int y) {
	dialog_.MoveObjectTo(object->GetId(), x, y);
}

void Application::RemoveObject(std::shared_ptr<const CellObject> object) {
	dialog_.RemoveObject(object->GetId());
}

void Application::LogicInit() {
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
	field->MakeGrass();
	for (int i = 0; i < 15; ++i) field->GrowPlants();
	field->BeginCycle();
	field->Play();
}

void Application::Init() {
	ConnectSignals();
	dialog_.show();
	LogicInit();	
}

QString Application::GetObjectType(std::shared_ptr<const CellObject> object) const {
	std::shared_ptr<const Flesh> flesh;
	QString obj_type;
	
	if (object->GetType(CellObject::Type::CARNIVORE)) { 
		obj_type = "carnivore";
	} else if (object->GetType(CellObject::Type::HERBIVORE)) {
		obj_type = "herbivore";
	} else if (object->GetType(CellObject::Type::PLANT)) {
		obj_type = "tree";
	} else if (object->GetType(CellObject::Type::FLESH)) {
		flesh = std::dynamic_pointer_cast<const Flesh>(object);
		if (flesh->IsCarnivore())
			obj_type = "carnivore_dead";
		else
			obj_type = "herbivore_dead";
	} else {
		obj_type = "stone";
	}
		
	return obj_type;
}

QString Application::GetGroundType(FieldCell::Ground type) const {
	QString surface;
	if (type == FieldCell::Ground::GRASS)
		surface = "grass";
	else
		surface = "soil";
	return surface;
}


void Application::RemoveSurfaceObject(const int x, const int y) {
	dialog_.RemoveSurfaceObject(x, y);
}

void Application::ConnectSignals() {
	connect(&dialog_, SIGNAL(NextLogicIteration()), this, SLOT(LogicIteration()));
	connect(&dialog_, SIGNAL(SpriteObjectClicked(int , int)), this, SLOT(SpriteObjectClicked(int , int)));
}

void Application::UpdateLog(const QString text) {
	dialog_.UpdateLog(text);
}

/**
 * @brief method used when sprite object was clicked (so stats window will be updated).
 * This method creates content of stats window.
 * param cell - Field's cell in which object, represented by clicked sprite, resides.
 */
boost::format Application::CreateStatistics(const int x, const int y) const {
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
	boost::format format = CreateStatistics(x, y);
	dialog_.UpdateStats(QString::fromStdString(format.str()));
}

Application::~Application() {
};
