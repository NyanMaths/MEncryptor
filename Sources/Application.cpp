#include <QApplication>

#include <fstream>

#include "Application.h"


////////////////////////////////////////  Initialize main window


Application::Application () : QTabWidget ()
{
    QFontDatabase::addApplicationFont ("Ubuntu.ttf");
    qApp->setFont (QFont ("Ubuntu", 12));


    std::string lang ("en");

    std::ifstream optionsFile ("UI Options.pastouche");
    if (optionsFile)
        getline (optionsFile, lang);

    translator = new QTranslator;
    translator->load ("mencryptor_" + QString::fromStdString (lang));
    qApp->installTranslator (translator);

    messageBoxesTranslator = new QTranslator;
    messageBoxesTranslator->load ("qtbase_" + QString::fromStdString (lang));
    qApp->installTranslator (messageBoxesTranslator);


    encryptorTab = new EncryptorWidget;
    addTab (encryptorTab, tr("Encrypt/Decrypt"));

    analyzerTab = new AnalyzerWidget;
    addTab (analyzerTab, tr("Frequency analyzer"));

    optionsTab = new OptionsWidget (encryptorTab);
    addTab (optionsTab, tr("Help and settings"));


    setWindowIcon (QIcon ("Window Icon.png"));


    show ();

    QSize screenSize = QGuiApplication::screens().at (0)->size ();
    move (screenSize.width () / 2 - width () / 2, screenSize.height () / 2 - height () / 2);

    setFixedSize (size ());
}

