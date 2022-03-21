#include <QApplication>
#include <QFontDatabase>
#include <QScreen>

#include <QStandardPaths>
#include <QFileInfo>
#include <QFile>
#include <QDir>

#include <QMessageBox>

#include "Application.hpp"


/*
The constructor retrieves the settings from a single file then initializes all tabs
*/
Application::Application () : QTabWidget ()
{
    QDir::setCurrent(QFileInfo(qApp->arguments().at(0)).absoluteDir().absolutePath() + "/..");
    readConfig();

    QFontDatabase::removeAllApplicationFonts();
    QFontDatabase::addApplicationFont("data/gui/ubuntu.ttf");

    qApp->setFont(QFont("ubuntu", 12));


    translator = new QTranslator;
    Q_UNUSED(translator->load("data/lang/mencryptor-" + config["language-locale"] + ".qm"));
    qApp->installTranslator(translator);

    messageBoxesTranslator = new QTranslator;
    Q_UNUSED(messageBoxesTranslator->load("data/lang/qtbase-" + config["language-locale"] + ".qm"));
    qApp->installTranslator(messageBoxesTranslator);


    encryptor = new EncryptorWidget(config);
    addTab(encryptor, tr("Ciphering"));
    setTabIcon(0, QIcon("data/gui/locker-closed.png"));

    frequencyAnalyzer = new AnalyzerWidget(config);
    addTab(frequencyAnalyzer, tr("Frequency analysis"));
    setTabIcon(1, QIcon("data/gui/graphs.png"));

    options = new OptionsWidget(config);
    addTab(options, tr("Settings"));
    setTabIcon(2, QIcon("data/gui/cogs.png"));


    qApp->setWindowIcon(QIcon("data/gui/logo.png"));
    qApp->setApplicationName(tr("MEncryptor - a set of ciphering tools"));


    QSize screenSize(QGuiApplication::screens().at(0)->size());
    resize(screenSize / 2);

    move((screenSize.width() - width()) / 2, (screenSize.height() - height()) / 2);

    show();
}

/*
The app configuration is stored in the standard path for user configuration,
typically ~/.config over Linux and C:/Users/username/AppData/Local over Windows,
...
And I do not have to answer to mac OS
*/
void Application::readConfig ()
{
    QFile configFile(QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).at(0) + "/mencryptor/general.getout");

    if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        if (!QFile::exists(QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).at(0) + "/mencryptor"))
            QDir(QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).at(0)).mkdir("mencryptor");

        configFile.setFileName("data/default-config.getout");

        if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text))
            QMessageBox::critical(nullptr, tr("Not my bad !"), tr("My installation seems to be broken, you may not give me the file access permission."));
    }

    QVector<QString> fileContent(QString(configFile.readAll()).split('\n'));

    for (qsizetype i(0) ; i + 1 < fileContent.length() ; i += 2)
        config[fileContent.at(i)] = fileContent.at(i + 1);
}


/*
The app configuration is stored in the standard path for user configuration,
typically ~/.config over Linux and C:/Users/username/AppData/Local over Windows,
...
And I do not have to answer to mac OS
*/
Application::~Application ()
{
    encryptor->getConfig(config);
    frequencyAnalyzer->getConfig(config);
    options->getConfig(config);

    QFile configFile(QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).at(0) + "/mencryptor/general.getout");

    if (configFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        QTextStream fileStream(&configFile);

        QVector<QString> configKeys(config.keys());
        for (qsizetype i(0) ; i != configKeys.length() ; i++)
            fileStream<<configKeys.at(i)<<'\n'<<config[configKeys.at(i)]<<(i + 1 == configKeys.length() ? "" : "\n");
    }
}
