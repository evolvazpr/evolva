#include "Field.hpp"

int main(void) {
	bool ret;
	std::shared_ptr<Object> sh_obj = std::make_shared<Three>();
	Field &field = Field::getInstance(10, 10);
	FieldCell &cell = field.getCell(4, 4);

	ret = cell.insertObject(sh_obj);
	std::cout << "cell.insertObject returns: " << ret << ". Should return 0.\n";
	ret = cell.insertObject(sh_obj);
	std::cout << "cell.insertObject returns: " << ret << ". Should return 1.\n";
	cell.removeObject();
	ret = cell.insertObject(sh_obj);
	std::cout << "cell.insertObject returns: " << ret << ". Should return 0.\n";
	ret = cell.insertObject(sh_obj);
	std::cout << "cell.insertObject returns: " << ret << ". Should return 1.\n";
	cell.removeObject();
}

