#include <QApplication>
#include "Execute.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Execute w;
    w.show();

    return a.exec();
}
