#include <QApplication>

#include "Application.hpp"


int main (int argc, char** argv)
{
    QApplication app(argc, argv);

    Application mainWindow;
    return app.exec();
}
