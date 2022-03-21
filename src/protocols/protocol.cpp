#include "src/protocols/protocol.hpp"


QWidget* Protocol::_mainWindow(nullptr);

Protocol::~Protocol ()
{
    // Nothing to be done here
}


QWidget* Protocol::mainWindow ()
{
    return _mainWindow;
}

void Protocol::setMainWindow (QWidget* newMainWindow)
{
    _mainWindow = newMainWindow;
}


QString Protocol::reversed (const QString& str)
{
    QString reversedStr;

    for (qsizetype i(str.length() - 1) ; i != -1 ; i--)
        reversedStr.push_back(str.at(i));

    return reversedStr;
}

