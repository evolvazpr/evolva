#include "Application.hpp"
#include <iostream>
#include <QMetaType>

Application::Application(int& argc, char **argv) : QApplication(argc, argv), gui_settings_("gui.xml"), logic_settings_("logic.xml"), 
						   dialog_(nullptr, logic_settings_["Field"]["width"], logic_settings_["Field"]["height"],
							   gui_settings_["Gui"]["pixels_per_object"] )  
{}

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
	if(!field_->Next()) {
		QMessageBox::information(nullptr, tr("Evolva"), tr("Simulation has finished."));
		exit();
	}
}

const QPixmap& Application::GetObjectPixmap(std::shared_ptr<const CellObject> object, uint* sprite_cnt) {
	std::string type = GetObjectType(object).toStdString();
	Pixmap pixmap = pixmaps_[type];
	*sprite_cnt = pixmap.GetSpriteCnt();
	return pixmap.GetPixmap();	
}

const QPixmap& Application::GetObjectPixmap(const FieldCell::Ground ground_type) {
	std::string type = GetGroundType(ground_type).toStdString();
	Pixmap pixmap = pixmaps_[type];
	return pixmap.GetPixmap();
}

void Application::CreateObject(std::shared_ptr<const CellObject> object, const int x, const int y) {
	uint sprite_cnt;
	const QPixmap& pixmap = GetObjectPixmap(object, &sprite_cnt);
	dialog_.CreateObject(object->GetId(), pixmap, sprite_cnt, x, y);
}

void Application::CreateSurfaceObject(const FieldCell::Ground ground_type, const int x, const int y) {
	const QPixmap& pixmap = GetObjectPixmap(ground_type);
	dialog_.CreateSurfaceObject(pixmap, x, y);
}

void Application::ReplaceSurfaceObject(const FieldCell::Ground ground_type, const int x, const int y) {
	const QPixmap& pixmap = GetObjectPixmap(ground_type);
	dialog_.ReplaceSurfaceObject(pixmap, x, y);
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

void Application::PrepareInitialObjects(CellObject::Type type) {
	typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
	
	std::shared_ptr<DnaCode> dna_ptr = std::make_shared<DnaCode>();
	std::string type_name;
	std::shared_ptr<Unit> unit;
	
	boost::char_separator<char> sep_registry(";");
	boost::char_separator<char> sep_data(",");
	
	int x, y;
	bool test;
	DnaCode &dna = *dna_ptr;
	if (type == CellObject::Type::HERBIVORE)
		type_name = "Herbivore";
	else
		type_name = "Carnivore";

	dna["intelligence"] = logic_settings_[type_name]["intelligence"];
	dna["agility"] = logic_settings_[type_name]["agility"];
	dna["speed"] = logic_settings_[type_name]["speed"];
	dna["strength"] = logic_settings_[type_name]["strength"];
	dna["death_time_u"] = logic_settings_[type_name]["death_time_u"];
	dna["death_time_s"] = logic_settings_[type_name]["death_time_s"];
	dna["mutability"] = logic_settings_[type_name]["mutability"];
	dna["mutation_stability"] = logic_settings_[type_name]["mutation_stability"];
	dna["fatigue_death_threshold"] = logic_settings_[type_name]["fatigue_death_threshold"];
	dna["health_death_threshold"] = logic_settings_[type_name]["health_death_threshold"];
	dna["fatigue_regeneration"] = logic_settings_[type_name]["fatigue_regeneration"];
	dna["health_regeneration_sleep"] = logic_settings_[type_name]["health_regeneration_sleep"];
	dna["health_regeneration"] = logic_settings_[type_name]["health_regeneration"];
	dna["requirements.normal_turn"] = logic_settings_[type_name]["requirements.normal_turn"];
	dna["requirements.sleep_turn"] = logic_settings_[type_name]["requirements.sleep_turn"];
	dna["requirements.eating"] = logic_settings_[type_name]["requirements.eating"];
	dna["requirements.mating"] = logic_settings_[type_name]["requirements.mating"];
	dna["requirements.step"] = logic_settings_[type_name]["requirements.step"];
	dna["requirements.attack"] = logic_settings_[type_name]["requirements.attack"];
	dna["requirements.childbirth"] = logic_settings_[type_name]["requirements.childbirth"];
	dna["wakeup_threshold"] = logic_settings_[type_name]["wakeup_threshold"];
	dna["fatigue_turn_increase"] = logic_settings_[type_name]["fatigue_turn_increase"];
	dna["normal_weight"] = logic_settings_[type_name]["normal_weight"];
	dna["pregnancy_time"] = logic_settings_[type_name]["pregnancy_time"];
	dna["herbivore"] = logic_settings_[type_name]["herbivore"];
	dna["carnivore"] = logic_settings_[type_name]["carnivore"];
	dna["vision"] = logic_settings_[type_name]["vision"];

	DnaGenerator gen(dna_ptr);
	gen.variability_ = logic_settings_[type_name]["gen.variability"];
	
	std::string temp = logic_settings_[type_name]["initial_coords"];
	
	tokenizer registry_tokens(temp, sep_registry);
	
	for (tokenizer::iterator it = registry_tokens.begin(); it != registry_tokens.end(); ++it) {
		tokenizer::iterator data_it;
		tokenizer data_tokens(*it, sep_data);
		data_it = data_tokens.begin();
		x = std::stoi(*data_it);
		++data_it;
		y = std::stoi(*data_it);
		unit = std::make_shared<Unit>(gen.Generate());
		test = field_->InsertObject(unit, x, y);
		if (!test)
			throw EvolvaException("Data in logic.xml is corrupted.");
	}
}

void Application::PrepareInitialPlants() {
	typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
	bool test;

	boost::char_separator<char> sep_registry(";");
	boost::char_separator<char> sep_data(",");

	const int initial_grow = logic_settings_["Plants"]["initial_grow"];	
	int x, y;
	double energy;

	std::shared_ptr<Tree> tree;
	std::string temp = logic_settings_["Plants"]["initial_params"];
	tokenizer registry_tokens(temp, sep_registry);

	for (tokenizer::iterator it = registry_tokens.begin(); it != registry_tokens.end(); ++it) {
		tokenizer::iterator data_it;
		tokenizer data_tokens(*it, sep_data);
		data_it = data_tokens.begin();
		energy = std::stod(*data_it);
		data_it++;
		x = std::stoi(*data_it);
		data_it++;
		y = std::stoi(*data_it);
		tree = std::make_shared<Tree>(energy);
		test = field_->InsertNmo(tree, x, y);
		if (!test)
			throw EvolvaException("Data in logic.xml is corrupted.");
	}	

	field_->MakeGrass();
	for (int i = 0; i < initial_grow; i++) {
		field_->GrowPlants();
	}
}

void Application::LogicInit() {
	field_ = Field::GetInstance(logic_settings_["Field"]["width"], logic_settings_["Field"]["height"]);
	PrepareInitialObjects(CellObject::Type::HERBIVORE);
	PrepareInitialObjects(CellObject::Type::CARNIVORE);	
	PrepareInitialPlants();
	field->BeginCycle();
	field->Play();
}

void Application::Init() {
	ConnectSignals();
	PixmapContainerInit();
	dialog_.show();
	LogicInit();	
}

void Application::AddToPixmaps(std::string name) {
	std::string path;
	uint spirte_cnt;
	Pixmap pixmap(gui_settings_["Gui"]["pixels_per_object"]);
	path = gui_settings_[name]["path"];
	sprite_cnt = gui_settings_[name]["sprite_cnt"];
	pixmap.SetObject(path, sprite_cnt);
	pixmap_container_.insert({name, pixmap});
}

void Application::PixmapContainerInit() {
	AddToPixmaps("carnivore");
	AddToPixmaps("herbivore");
	AddToPixmaps("carnivore_dead");
	AddToPixmaps("herbivore_dead");
	AddToPixmaps("tree");
	AddToPixmaps("stone");
	AddToPixmaps("grass");
	AddToPixmaps("soil");
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

void Application::MoveLogicToEndOfRound() {
	do {
		if(!field_->Next()) {
			QMessageBox::information(nullptr, tr("Evolva"), tr("Simulation has finished."));
			exit();
		}
	} while (!field_->IsNewTurn());	
}

void Application::RemoveSurfaceObject(const int x, const int y) {
	dialog_.RemoveSurfaceObject(x, y);
}

void Application::ConnectSignals() {
	connect(&dialog_, SIGNAL(NextLogicIteration()), this, SLOT(LogicIteration()));
	connect(&dialog_, SIGNAL(SpriteObjectClicked(int , int)), this, SLOT(SpriteObjectClicked(int , int)));
	connect(&dialog_, SIGNAL(MoveToTheEndOfRound()), this, SLOT(MoveLogicToEndOfRound()));
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
