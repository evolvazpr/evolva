#include "Field.hpp"
#include "CellObject.hpp"


int main(void) {
	std::shared_ptr<Field> field = Field::GetInstance(10, 10);
	std::shared_ptr<MovableObject> mov_obj = std::make_shared<MovableObject>(0, 3);
	bool ret = field->InsertObject(mov_obj, 3, 3);
	mov_obj = std::make_shared<MovableObject>(1, 3);
	ret = field->InsertObject(mov_obj, 4, 3);
	mov_obj = nullptr;
	field->IterateOverMovableObjects();
	return 0;
}
