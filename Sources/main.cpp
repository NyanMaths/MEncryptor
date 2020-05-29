#include <QApplication>

#include "Application.h"


int main (int argc, char** argv)
{
    QApplication app (argc, argv);

    Application mainWindow;
    return app.exec();
}
