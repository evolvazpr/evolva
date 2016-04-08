#ifndef STATWINDOW_HPP
#define STATWINDOW_HPP

#include <QObject>
#include <QWidget>

class StatWindow
{
public:
    StatWindow();
    void StatDisplay(size_t); //displays stats of the selected object(is id_ sufficient? does it unequivocally describe one given object, or is there more info required?)
    ~StatWindow();
};

#endif // STATWINDOW_HPP
