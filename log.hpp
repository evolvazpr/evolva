#ifndef LOG_HPP
#define LOG_HPP
#include <QString>


class Log
{
public:
    void LogMessage(); //displays log message in the log box. id1_ - first object; id2_ - second object. Interaction between more than 2?
    Log();
    ~Log();
private:
    QString message_; //is this needed? there's already a method for displaying the message
};

#endif // LOG_HPP
