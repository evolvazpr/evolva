#include "Field.hpp"

std::shared_ptr<Field> Field::instance_ = std::shared_ptr<Field>(nullptr);

Field::Field(const int x, const int y) : x_max_(x), y_max_(y), cells_(boost::extents[x][y]) {
	for (int x_i = 0; x_i < x; x_i++) {
		for (int y_i = 0; y_i < y; y_i++) {
			cells_[x_i][y_i] = std::make_shared<FieldCell>();
		}
	}	
}	

Field& Field::GetInstance(const int x, const int y) {
	if (!instance_) {
			//TODO: replace std;:shared_ptr with std::make_shared
				instance_ = std::shared_ptr<Field>(new Field(x, y));
			}
			return *instance_; 
}

std::weak_ptr<FieldCell> Field::GetCell(std::pair<int, int> coordinates) {
	return std::weak_ptr<FieldCell>(cells_[coordinates.first][coordinates.second]);
}

bool Field::MoveObject(std::shared_ptr<CellObject> object, 
											 std::pair<int, int> initial_real_coordinates,
								 			 std::pair<int, int> final_relative_coordinates, 
											 std::pair<int, int> final_real_coordinates) {
	std::shared_ptr<CellObject> cell = std::shared_ptr<CellObject>(GetCell(initial_real_coordinates));	
	return true;
}
