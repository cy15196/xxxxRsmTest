#ifndef TOPTESTAPPLICATION_H
#define TOPTESTAPPLICATION_H

#include <QApplication>

class TopTestApplication : public QApplication
{
public:
    TopTestApplication(int &argc, char **argv);
    ~TopTestApplication(){}
private:
    virtual bool notify( QObject* receiver, QEvent* e );
};

#endif // TOPTESTAPPLICATION_H
