#ifndef DIALOG_HPP
#define DIALOG_HPP

#include <QDialog>
#include <QGraphicsObject>
#include <QTimer>

namespace Ui {
class Dialog;
}



class Dialog : public QDialog
{
    Q_OBJECT

private:

    Ui::Dialog *ui;
    QGraphicsScene *scene;
    const unsigned int width_;
    const unsigned int height_;

    QGraphicsItem* searchObject(const uint id);
    qreal calculate_x(const uint x);
    qreal calculate_y(const uint y);
    qreal calculate_radius();

    QTimer timer;


public:

    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    void createObject(const uint id, const uint x, const uint y);
    void moveObject(const uint id, const uint x, const uint y);



private slots:
    void on_pushButton_clicked();
    void test();
};

#endif // DIALOG_HPP
