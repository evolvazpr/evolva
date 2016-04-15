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

int main(void) {

	field = Field::GetInstance(10, 10);



	std::shared_ptr<DnaCode> dna_ptr = std::make_shared<DnaCode>();
	DnaCode &dna = *dna_ptr;
	dna["intelligence"] = 45.0;
	dna["agility"] = 22.0;
	dna["speed"] = 31.0;
	dna["strength"] = 68.4;
	dna["death_time_u"] = 50.0;
	dna["death_time_s"] = 2.68;
	dna["mutability"] = 10.0;
	dna["mutation_stability"] = 1.1;
	dna["fatigue_death_threshold"] = 80.0;
	dna["health_death_threshold"] = 20.0;
	dna["fatigue_regeneration"] = 10.0;
	dna["health_regeneration_sleep"] = 5.0;
	dna["requirements.normal_turn"] = 10.0;
	dna["requirements.sleep_turn"] = 3.0;
	dna["requirements.eating"] = 2.0;
	dna["requirements.mating"] = 25.0;
	dna["requirements.step"] = 1.0;
	dna["requirements.attack"] = 50.0;
	dna["requirements.childbirth"] = 80.0;
	dna["wakeup_threshold"] = 20.0;
	dna["fatigue_turn_increase"] = 2.0;
	dna["normal_weight"] = 80.0;
	dna["pregnancy_time"] = 2.0;
	dna["herbivore"] = 100.0;
	dna["carnivore"] = 0.0;

	DnaGenerator gen(dna_ptr);
	gen.variability_ = 30.0;

	std::shared_ptr<Unit> u[2];
	u[0] = std::make_shared<Unit>(gen.Generate());
	u[1] = std::make_shared<Unit>(gen.Generate());

	u[0]->energy_ = 200.0;
	u[0]->death_ = 10000;
	u[0]->dna_["normal_weight"] = 220.0;
	field->InsertObject(u[0], 0, 0);
	u[1]->energy_ = 200.0;
	u[1]->death_ = 10000;
	u[1]->dna_["normal_weight"] = 200.0;
	field->InsertObject(u[1], 9, 9);


	for (size_t i = 0; i < 2; ++i) {
		if (!u[i]->GetType(CellObject::Type::UNIT)) continue;
		std::cout << "u " << i << ":\n";
		for (auto j = u[i]->dna_.begin(); j != u[i]->dna_.end(); ++j) {
			std::cout << j->first << ": " << j->second << "\n";
		}
		std::cout << "death: " << u[i]->death_ << "\n";
		std::cout << "energy: " << u[i]->energy_ << "\n";
		std::cout << "c. speed: " << u[i]->speed_ << "\n";
		std::cout << "x: " << u[i]->GetX() << "\n";
		std::cout << "y: " << u[i]->GetY() << "\n";
		std::cout << "\n";
	}


	field->InsertNmo(std::make_shared<Tree>(50.0), 1, 1);
	field->InsertNmo(std::make_shared<Tree>(80.0), 7, 2);
	field->InsertNmo(std::make_shared<Tree>(20.0), 3, 4);
	field->InsertNmo(std::make_shared<Tree>(24.0), 4, 4);
	field->InsertNmo(std::make_shared<Tree>(500.0), 2, 8);
	field->InsertNmo(std::make_shared<Tree>(100.0), 0, 9);/**/


//	field->KillNmo(field->GetCell(1,1)->GetNmo());
//	field->KillNmo(field->GetCell(3,4)->GetNmo());
//	field->KillNmo(field->GetCell(2,8)->GetNmo());

	Tui tui;


	field->BeginCycle();
	field->Play();

	Unit *xz = u[0].get();
	xz->IsRemoved();

	while(1) {
	//	if(!u[0]->IsRemoved()) {
			tui.PrintField();

//			std::cout << "0\n" << "en: " << u[0]->GetEnergy() << "\nfatg: " << u[0]->GetFatigue() << "\n";
//			std::cout << "1\n" << "en: " << u[1]->GetEnergy() << "\nfatg: " << u[1]->GetFatigue() << "\n";
		field->f2();
	//	}
//		else std::cout << "COUNT: " << u[0].use_count() << "\n" << u[1].use_count() << "\n";
		if (getch() == ' ') break;
		if (!field->Next()) break;
	}
	std::cout << "kuniec, c:" << u[0].use_count() << "\n" << u[1].use_count() << "\n\n";


	return 0;
}
