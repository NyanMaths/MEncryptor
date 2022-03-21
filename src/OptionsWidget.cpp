#include <QApplication>
#include <QProcess>

#include <QMessageBox>

#include "OptionsWidget.hpp"


///////////////////  OptionsWidget initialization  ///////////////////


OptionsWidget::OptionsWidget (const QMap<QString, QString>& config) : QWidget (),

    layout(new QGridLayout(this)),


    aboutMEncryptorLabel(new QLabel(tr(
        "This opensource software is actively developed by a geek nicknamed NY4N_M4THS<br/>"
        "under <a href = 'https://www.gnu.org/licenses/gpl-3.0.en.html'>license GPL 3.0</a> with the framework <a href = 'https://qt.io'>Qt 6</a> in C++ language.<br/>"
        "You can visit <a href = 'https://memetech-inc.weebly.com'>my website</a> to check for updates,<br/>"
        "try other applications or ask for new features !<br/>"
        "Click <a href = 'https://github.com/NyanMaths/MEncryptor'>here</a> to visit the GitHub repository of the project."))),


    UIOptionsBox(new QGroupBox(tr("UI options"))),
    UIOptionsBoxLayout(new QGridLayout(UIOptionsBox)),

    chooseLanguageLabel(new QLabel(tr("Display language :"))),
    languageSelecter(new QComboBox),

    chooseThemeLabel(new QLabel(tr("Interface theme :"))),
    themeSelecter(new QComboBox)
{
    aboutMEncryptorLabel->setOpenExternalLinks(true);
    aboutMEncryptorLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    aboutMEncryptorLabel->setTextFormat(Qt::RichText);


    UIOptionsBoxLayout->setAlignment(Qt::AlignLeft);

    languageSelecter->addItem("English", QVariant("en"));
    languageSelecter->addItem("Français", QVariant("fr"));

    themeSelecter->addItem(tr("Light"), QVariant("light"));
    themeSelecter->addItem(tr("Dark"), QVariant("dark"));


    UIOptionsBoxLayout->addWidget(chooseLanguageLabel, 0, 0);
    UIOptionsBoxLayout->addWidget(languageSelecter, 0, 1);
    UIOptionsBoxLayout->addWidget(chooseThemeLabel, 1, 0);
    UIOptionsBoxLayout->addWidget(themeSelecter, 1, 1);

    layout->addWidget(aboutMEncryptorLabel, 0, 0, 1, 2);
    layout->addWidget(UIOptionsBox, 2, 0);


    loadConfig(config);
}

void OptionsWidget::initDarkPalette ()
{
    QColor darkColor = QColor(45, 45, 45);
    QColor disabledColor = QColor(127, 127, 127);

    darkPalette.setColor(QPalette::Window, darkColor);
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(18, 18, 18));
    darkPalette.setColor(QPalette::AlternateBase, darkColor);
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::Text, disabledColor);
    darkPalette.setColor(QPalette::Button, darkColor);
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, disabledColor);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    darkPalette.setColor(QPalette::Disabled, QPalette::HighlightedText, disabledColor);

    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
}

/*
I prefer connecting signals and slots after loading the configuration because some slots could be called uselessly.
*/
void OptionsWidget::loadConfig (const QMap<QString, QString>& config)
{
    languageSelecter->setCurrentText(config["language"]);
    themeSelecter->setCurrentIndex(config["theme-index"].toInt());


    if (themeSelecter->currentData().toString() == "dark")
    {
        initDarkPalette();

        qApp->setPalette(darkPalette);
    }


    connect(languageSelecter, &QComboBox::currentIndexChanged, this, &OptionsWidget::promptToRestart);
    connect(themeSelecter, &QComboBox::currentIndexChanged, this, &OptionsWidget::promptToRestart);
}


void OptionsWidget::getConfig (QMap<QString, QString>& config)
{
    config["language"] = languageSelecter->currentText();
    config["language-locale"] = languageSelecter->currentData().toString();
    config["theme-index"] = QString::number(themeSelecter->currentIndex());
}


///////////////////  Untitled  ///////////////////


void OptionsWidget::promptToRestart ()
{
    if (QMessageBox::question(this, tr("Restart needed"), tr("You need to reload the application to apply changes.\nDo you want to restart now ?")) == QMessageBox::Yes)
    {
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    }
}
