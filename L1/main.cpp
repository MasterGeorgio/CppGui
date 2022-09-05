#include "exview.h"

#include <QApplication>
#include "MathComputation.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MathComputation w;
    w.show();

    return a.exec();
}
