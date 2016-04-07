#ifndef DIALOG_HPP
#define DIALOG_HPP

#include <QDialog>
#include <QGraphicsObject>

namespace Ui {
class Dialog;
}

class DrawingInterface {
private:
       QGraphicsScene *scene;
       const unsigned int field_width_;
       const unsigned int field_heigth_;
public:
       DrawingInterface(QGraphicsScene *scene, const unsigned int field_width,
                        const unsigned int field_heigth);
       void insertObject(QGraphicsObject *object, unsigned int x, unsigned int y);
       void removeObject(QGraphicsObject *object);
       void draw(QGraphicsObject *object, unsigned int x, unsigned int y);
};

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void drawSand();
private slots:
    void on_pushButton_clicked();

private:
    Ui::Dialog *ui;
    QGraphicsScene *scene;
    DrawingInterface drawing_interface_;
};

#endif // DIALOG_HPP
