#include <QApplication>
#include "Snake.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    (new Snake())->show();

    return a.exec();
}