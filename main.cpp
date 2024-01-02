#include <QApplication>
#include "ui/editor.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    Editor ed;
    ed.show();

    return QApplication::exec();
}
