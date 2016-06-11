#include "Application.hpp"
#include <iostream>
#include <QMetaType>
#include <iomanip>
#include "Statistics.hpp"


Pixmap::Pixmap(const uint pixels_per_object) {
		pixels_per_object_ = pixels_per_object;
}
	
Pixmap::Pixmap(const Pixmap & p) {
		pixels_per_object_ = p.pixels_per_object_;
		pixmap_ = p.pixmap_;
		sprite_cnt_ = p.sprite_cnt_;
}

Pixmap::Pixmap(Pixmap&& p) {
		pixels_per_object_ = p.pixels_per_object_;
		pixmap_ = p.pixmap_;
		sprite_cnt_ = p.sprite_cnt_;
}

void Pixmap::SetObject(QString path, uint sprite_cnt) {
		pixmap_ = QPixmap(path);
		sprite_cnt_ = sprite_cnt;
		pixmap_ = pixmap_.scaled(pixels_per_object_ * sprite_cnt_, pixels_per_object_ * sprite_cnt_, 
					 Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

const QPixmap& Pixmap::GetPixmap() const { return pixmap_; }
uint Pixmap::GetSpriteCnt() const { return sprite_cnt_; }


Application::Application(int& argc, char **argv) : QApplication(argc, argv), gui_settings_("gui.xml"), logic_settings_("logic.xml"), 
						   dialog_(nullptr, logic_settings_["Field"]["width"], logic_settings_["Field"]["height"],
						   gui_settings_["Gui"]["pixels_per_object"] )  
{}

Application* Application::instance_ = nullptr;

/** 
 * @brief: method to get instance of application.
 * It was needed to make this as a singleton, because in Field there was need to get
 * handler to object of this class.
 */
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

/**
 * @brief: method called from dialog when button is clicked. 
 */
void Application::LogicIteration() {
	if(!field_->Next()) {
		QMessageBox::information(nullptr, tr("Evolva"), tr("Simulation has finished."));
		exit();
	}
	dialog_.UpdateOverallStatistics();
}

/**
 * @brief: method to get sprite.
 */
const QPixmap& Application::GetObjectPixmap(std::shared_ptr<const CellObject> object, uint* sprite_cnt) const {
	const std::string type = GetObjectType(object).toStdString();
	*sprite_cnt = pixmaps_.at(type).GetSpriteCnt();
	return pixmaps_.at(type).GetPixmap();	
}

/**
 * @brief: method to get sprite.
 */
const QPixmap& Application::GetObjectPixmap(const FieldCell::Ground ground_type) const {
	const std::string type = GetGroundType(ground_type).toStdString();
	return pixmaps_.at(type).GetPixmap();
}

/**
 * @brief: Method creates graphical representation of object.
 * @param object
 * @param x
 * @param y
 */
void Application::CreateObject(std::shared_ptr<const CellObject> object, const int x, const int y) {
	uint sprite_cnt;
	const QPixmap& pixmap = GetObjectPixmap(object, &sprite_cnt);
	dialog_.CreateObject(object->GetId(), pixmap, sprite_cnt, x, y);
}


/**
 * @brief: Method creates graphical representation of surface.
 * @param ground_type
 * @param x
 * @param y
 */
void Application::CreateSurfaceObject(const FieldCell::Ground ground_type, const int x, const int y) {
	const QPixmap& pixmap = GetObjectPixmap(ground_type);
	dialog_.CreateSurfaceObject(pixmap, x, y);
}

/**
 * @brief: Method replaces surface types and updates gui. Used to speed up update of grass/soil.
 * @param ground_type
 * @param x
 * @param y
 */
void Application::ReplaceSurfaceObject(const FieldCell::Ground ground_type, const int x, const int y) {
	const QPixmap& pixmap = GetObjectPixmap(ground_type);
	dialog_.ReplaceSurfaceObject(pixmap, x, y);
}

/**
 * @brief: Proxy method between logic and gui to move object to specific destiny.
 * @param object
 * @param x
 * @param y
 */
void Application::MoveObject(std::shared_ptr<const CellObject> object, const int x, const int y) {
	dialog_.MoveObject(object->GetId(), x, y);
}

/**
 * @brief: Proxy method between logic and gui. x and y are relative coordinates.
 * @param object
 * @param x
 * @param y
 */
void Application::MoveObjectTo(std::shared_ptr<const CellObject> object, const int x, const int y) {
	dialog_.MoveObjectTo(object->GetId(), x, y);
}

/**
 * @brief: Proxy method between logic and gui. Removes object from field.
 * @param object
 */
void Application::RemoveObject(std::shared_ptr<const CellObject> object) {
	dialog_.RemoveObject(object->GetId());
}

/**
 * @brief: Method which prepares one of a race.
 * It loads settings rom logic.xml file. This file must be in the same directory as
 * the executable!!!
 * @param type - type of object to prepare.
 */
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

/**
 * @brief: Method prepares plants. It loads data from logic.xml file! This file must be in the same
 * directory as the executable!
 */
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
	dialog_.UpdateOverallStatistics();
	field->BeginCycle();
	field->Play();
}

/**
 * @brief Method to initalize program.
 * @param logic - if true - initalizes also logic. If false - logic (Field, all units, trees, surface of field
 * is not initalized).
 */
void Application::Init(bool logic) {
	ConnectSignals();
	PixmapContainerInit();
	if (logic) {
		dialog_.show();
		LogicInit();
	}	
}

void Application::AddToPixmaps(std::string name) {
	uint sprite_cnt;
	std::string path = gui_settings_[name]["path"];
	uint pixels_per_object = gui_settings_["Gui"]["pixels_per_object"];
	Pixmap pixmap(pixels_per_object);
	sprite_cnt = gui_settings_[name]["sprite_cnt"];
	pixmap.SetObject(QString::fromStdString(path), sprite_cnt);
	pixmaps_.insert({name, pixmap});
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

/**
 * @brief Method called from dialog, via next round(s) button.
 * @param rounds - repetition of method.
 */
void Application::MoveLogicToEndOfRound(uint rounds) {
	for (uint i = 0; i < rounds; i++) {
		do {
			if(!field_->Next()) {
				QMessageBox::information(nullptr, tr("Evolva"), tr("Simulation has finished."));
				exit();
			}
		} while (!field_->IsNewTurn());	
	}
	dialog_.UpdateOverallStatistics();
}

/**
 * @brief proxy method between logic and gui.
 */
void Application::RemoveSurfaceObject(const int x, const int y) {
	dialog_.RemoveSurfaceObject(x, y);
}

void Application::ConnectSignals() {
	connect(&dialog_, SIGNAL(NextLogicIteration()), this, SLOT(LogicIteration()));
	connect(&dialog_, SIGNAL(SpriteObjectClicked(int , int)), this, SLOT(SpriteObjectClicked(int , int)));
	connect(&dialog_, SIGNAL(MoveToTheEndOfRound(uint)), this, SLOT(MoveLogicToEndOfRound(uint)));
}

/**
 * @brief proxy method between logic and gui to update log window.
 */
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

/**
 * @brief method called by dialog, when sprite object was clicked. It fills stats window.
 * @param x - x position of object.
 * @param y - y position of object.
 */
void Application::SpriteObjectClicked(int x, int y) {
	boost::format format = CreateStatistics(x, y);
	dialog_.UpdateStats(QString::fromStdString(format.str()));
}

Application::~Application() {
};
