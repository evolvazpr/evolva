#include "Field.hpp"
#include "CellObject.hpp"
#include "Tui.hpp"
#define private public
#define protected public
#include "DnaGenerator.hpp"
#include "Unit.hpp"
#undef protected
#undef private
#include <iostream>
#include <iomanip>
#include "Statistics.hpp"


    #include <unistd.h>   //_getch*/
    #include <termios.h>  //_getch*/



char getch(){
    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
//    printf("%c\n",buf);
    return buf;
 }


#include "CyclicQueue.hpp"


std::shared_ptr<Field> field;

int main(void) {

	field = Field::GetInstance(50, 35);

	{

		std::shared_ptr<DnaCode> dna_ptr = std::make_shared<DnaCode>();
		DnaCode &dna = *dna_ptr;
		dna["intelligence"] = 65.0;
		dna["agility"] = 30.0;
		dna["speed"] = 80.0;
		dna["strength"] = 15.4;
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
		dna["requirements.childbirth"] = 20.0;
		dna["wakeup_threshold"] = 20.0;
		dna["fatigue_turn_increase"] = 2.0;
		dna["normal_weight"] = 120.0;
		dna["pregnancy_time"] = 1.49;
		dna["herbivore"] = 100.0;
		dna["carnivore"] = 2.0;
		dna["vision"] = 15.0;

		DnaGenerator gen(dna_ptr);
		gen.variability_ = 30.0;

		std::shared_ptr<Unit> u[11];
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
		u[10] = std::make_shared<Unit>(gen.Generate());


//		u[0]->energy_ = 200.0;
//		u[0]->death_ = 100;
//		u[0]->dna_["normal_weight"] = 220.0;
		field->InsertObject(u[0], 0, 0);
		field->InsertObject(u[1], 9, 9);
		field->InsertObject(u[2], 7, 0);
		field->InsertObject(u[3], 11, 1);
		field->InsertObject(u[4], 10, 2);
		field->InsertObject(u[5], 9, 3);
		field->InsertObject(u[6], 2, 0);
		field->InsertObject(u[7], 22, 2);
		field->InsertObject(u[8], 24, 0);
		field->InsertObject(u[9], 1, 1);
		field->InsertObject(u[10], 24, 2);/**/

	}
	{

		std::shared_ptr<DnaCode> dna_ptr = std::make_shared<DnaCode>();
		DnaCode &dna = *dna_ptr;
		dna["intelligence"] = 65.0;
		dna["agility"] = 60.0;
		dna["speed"] = 80.0;
		dna["strength"] = 80.4;
		dna["death_time_u"] = 60.0;
		dna["death_time_s"] = 1.5;
		dna["mutability"] = 20.0;
		dna["mutation_stability"] = 1.1;
		dna["fatigue_death_threshold"] = 95.0;
		dna["health_death_threshold"] = 0.0;
		dna["fatigue_regeneration"] = 15.0;
		dna["health_regeneration_sleep"] = 10.0;
		dna["health_regeneration"] = 0.0;
		dna["requirements.normal_turn"] = 12.0;
		dna["requirements.sleep_turn"] = 1.0;
		dna["requirements.eating"] = 5.0;
		dna["requirements.mating"] = 20.0;
		dna["requirements.step"] = 1.0;
		dna["requirements.attack"] = 30.0;
		dna["requirements.childbirth"] = 60.0;
		dna["wakeup_threshold"] = 20.0;
		dna["fatigue_turn_increase"] = 4.0;
		dna["normal_weight"] = 800.0;
		dna["pregnancy_time"] = 2.3;
		dna["herbivore"] = 2.0;
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
		u[5] = std::make_shared<Unit>(gen.Generate());		/**/

//		u[0]->energy_ = 2000.0;
//		u[0]->death_ = 100;
//		u[0]->dna_["normal_weight"] = 2200.0;
		field->InsertObject(u[0], 19, 19);
//		u[1]->energy_ = 2000.0;
//		u[1]->death_ = 100;
//		u[1]->dna_["normal_weight"] = 2000.0;
		field->InsertObject(u[1], 40, 15);
		field->InsertObject(u[2], 39, 15);
		field->InsertObject(u[3], 38, 15);
		field->InsertObject(u[4], 40, 18);
		field->InsertObject(u[5], 39, 18);

	}





	field->InsertNmo(std::make_shared<Tree>(50.0), 1, 2);
	field->InsertNmo(std::make_shared<Tree>(80.0), 7, 2);
	field->InsertNmo(std::make_shared<Tree>(20.0), 3, 4);
	field->InsertNmo(std::make_shared<Tree>(24.0), 4, 4);
	field->InsertNmo(std::make_shared<Tree>(500.0), 2, 8);
	field->InsertNmo(std::make_shared<Tree>(100.0), 0, 9);/**/


//	field->KillNmo(field->GetCell(1,1)->GetNmo());
//	field->KillNmo(field->GetCell(3,4)->GetNmo());
//	field->KillNmo(field->GetCell(2,8)->GetNmo());

	Tui tui;
	field->MakeGrass();
	for (int i = 0; i < 30; ++i) field->GrowPlants();

	field->BeginCycle();
	field->Play();

//	Unit *xz = u[0].get();
//	xz->IsRemoved();

	bool day = false;
	while(1) {
		tui.PrintField();
		std::cout << std::setw(16) << "carnies: " << field->stats_->carnivore_.Get() << "";
		std::cout << std::setw(16) << "herbis: " << field->stats_->herbivore_.Get() << "";
		std::cout << std::setw(16) << "omnis: " << field->stats_->omnivore_.Get() << "\n";
		std::cout << std::setw(16) << "disabled: " << field->stats_->disabled_.Get() << "";
		std::cout << std::setw(16) << "trees: " << field->stats_->tree_.Get() << "";
		std::cout << std::setw(16) << "flesh: " << field->stats_->flesh_.Get() << "\n";
		std::cout << std::setw(16) << "miscarries: " << field->stats_->miscarry_.GetSum() << "";
		std::cout << std::setw(16) << "fights: " << field->stats_->fight_.GetSum() << "";
		std::cout << std::setw(16) << "escapes: " << field->stats_->escape_.GetSum() << "\n";
	//	field->f2();
//		field->LogAllUnits();
		char c = getch();
		if (c == 'a') day = true;
		else if (c == 'b') day = false;
		else if (c == ' ') break;
		if (!field->Next()) break;
		if (day && field->IsNewTurn()) {
			std::string dumm;
			std::getline(std::cin, dumm);
		}
	}
//	std::cout << "kuniec, c:" << u[0].use_count() << "\n" << u[1].use_count() << "\n\n";


	return 0;
}
