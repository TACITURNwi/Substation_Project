#include <QCoreApplication>
#include <qdebug.h>
#include "manager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    manager my_manager;
    my_manager.start();
    return a.exec();
}
