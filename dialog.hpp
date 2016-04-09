#ifndef DIALOG_HPP
#define DIALOG_HPP

#include <QDialog>
#include <QGraphicsObject>
#include <QTimer>

#include <cmath>

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
    int calculate_x(const uint x);
    int calculate_y(const uint y);
    int calculate_radius();

    QTimer timer;


public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    void createObject(const uint id, const uint x, const uint y);
    void moveObject(const uint id, const int x, const int y);
    void moveObjectTo(const uint id, const int x, const int y);
    void removeObject(const uint id);

private slots:
    void on_pushButton_clicked();
    void test();
};

#endif // DIALOG_HPP
