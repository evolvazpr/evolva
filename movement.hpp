#ifndef MOVEMENT_HPP
#define MOVEMENT_HPP

#include <QObject>
#include <QWidget>

//GUI movement of objects around the GraphicsView

class Movement
{
public:
    Movement();
    bool MoveObject (const size_t , size_t , int , int); //move object of object_type type to int coordinates specified by final_x_real and final_y_real
    bool InsertObject (const size_t , int , int ); //insert object with an id_, in the final_x_real and final_y_real coordinates
    bool ChangeType (const size_t , size_t ); //change type of displayed object, specified by its id_ and object_type(no need for coordinates?)
    ~Movement();
};

#endif // MOVEMENT_HPP
