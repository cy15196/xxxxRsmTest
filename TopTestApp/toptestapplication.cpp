#include "toptestapplication.h"
#include <iostream>

#include <QDebug>

TopTestApplication::TopTestApplication(int &argc, char **argv)
    :QApplication(argc,argv)
{

}

bool TopTestApplication::notify(QObject *receiver, QEvent *e)
{
    bool errRet = false;
    try
    {
//        qDebug()<<"receiver"<<receiver<<"event"<<e;
        errRet = QApplication::notify (receiver, e);
    }
    catch( std::exception& e )
    {
        std::cout << "TopTestApplication: exception occurred : "
                  << e.what() << std::endl;
    }
    catch( std::exception* e )
    {
        std::cout << "TopTestApplication: exception occurred : "
                  << e->what() << std::endl;
    }
    catch(...)
    {
        std::cout << "TopTestApplication: exception occurred. "
                  << std::endl;
    }
    return errRet;
}
