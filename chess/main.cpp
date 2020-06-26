#include <QApplication>
#include"board.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    board b;
    b.show();


    return a.exec();
}
