#include "mainwindow.h"
#include <QApplication>
#include "input.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //при запуске программы первым делом открываем окно авторизации
    Input w;
    w.show();

    return a.exec();
}
