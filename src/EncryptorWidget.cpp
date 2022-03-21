#include <QApplication>
#include <QScreen>

#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>

#include "TranslationReportDialog.hpp"

#include "protocols/leet-speak.hpp"
#include "protocols/cyrillic.hpp"
#include "protocols/greek.hpp"
#include "protocols/braille.hpp"
#include "protocols/values.hpp"
#include "protocols/reversed-sentence.hpp"
#include "src/protocols/reversed-words.hpp"
#include "protocols/unicode-values.hpp"
#include "protocols/shift.hpp"
#include "protocols/vigenere.hpp"
#include "protocols/morse.hpp"

#include "EncryptorWidget.hpp"


///////////////////  EncryptorWidget initialization  ///////////////////


EncryptorWidget::EncryptorWidget (const QMap<QString, QString>& config) : QWidget (),

    protocolsNames
    {
        { "leet-speak", tr("Leet Speak (1337)") },
        { "cyrillic", tr("Cyrillic") },
        { "greek", tr("Greek") },
        { "braille", tr("Braille") },
        { "values", tr("Values (A-1 Z-26 Space-0)") },
        { "reversed-sentence", tr("Reversed sentence") },
        { "reversed-words", tr("Reversed words") },
        { "unicode-values", tr("Unicode values") },
        { "shift", tr("Shift") },
        { "vigenere", tr("Vigenère") },
        { "morse", tr("Morse") }
    },
    engines
    {
        { "leet-speak", new LeetSpeak },
        { "cyrillic", new Cyrillic },
        { "greek", new Greek },
        { "braille", new Braille },
        { "values", new Values },
        { "reversed-sentence", new ReversedSentence },
        { "reversed-words", new ReversedWords },
        { "unicode-values", new UnicodeValues },
        { "shift", new Shift },
        { "vigenere", new Vigenere },
        { "morse", new Morse }
    },

    errors
    {
        { Protocol::Error::MissingDataFile, tr("Some files are missing in the app directory,\nreinstall it should fix the problem !") },
        { Protocol::Error::CorruptedMessage, tr("The message that you try to encrypt/decrypt\nis corrupted (Forbidden symbols, etc...) !") },
        { Protocol::Error::InvalidKey, tr("You entered an invalid key, please retry !") },
    },


    layout(new QGridLayout(this)),


    inputBox(new QGroupBox("Message")),
    inputBoxLayout(new QVBoxLayout(inputBox)),
    inputArea(new QTextEdit),

    protocolSelecter(new QComboBox),

    cipherButton(new QPushButton(QIcon(QPixmap("data/gui/locker-closed.png")), tr("Cipher message"))),
    decipherButton(new QPushButton(QIcon(QPixmap("data/gui/locker-opened.png")), tr("Decipher message")))
{
    Protocol::setMainWindow(this);


    inputArea->setAcceptRichText(false);

    initProtocols();


    inputBoxLayout->addWidget(inputArea);

    layout->addWidget(inputBox, 0, 0, 1, 3);
    layout->addWidget(protocolSelecter, 1, 0);
    layout->addWidget(cipherButton, 1, 1);
    layout->addWidget(decipherButton, 1, 2);


    loadConfig(config);
}

void EncryptorWidget::initProtocols ()
{
    QVector<QString> protocolsIdentifiers(protocolsNames.keys());
    for (qsizetype i(0) ; i != protocolsIdentifiers.length() ; i++)
        protocolSelecter->addItem(protocolsNames[protocolsIdentifiers.at(i)], QVariant(protocolsIdentifiers.at(i)));
}

/*
I prefer connecting signals and slots after loading the configuration because some slots could be called uselessly.
*/
void EncryptorWidget::loadConfig (const QMap<QString, QString>& config)
{
    protocolSelecter->setCurrentText(protocolsNames[config["encryption-protocol"]]);


    connect(cipherButton, &QPushButton::clicked, this, &EncryptorWidget::cipher);
    connect(decipherButton, &QPushButton::clicked, this, &EncryptorWidget::cipher);
}


void EncryptorWidget::getConfig (QMap<QString, QString>& config)
{
    config["encryption-protocol"] = protocolSelecter->currentData().toString();
}

EncryptorWidget::~EncryptorWidget ()
{
    // Free memory of ciphering engines

    for (auto engine : engines)
        delete engine;
}


///////////////////  Ciphering methods  ///////////////////


void EncryptorWidget::cipher ()
{
    encryptorOutput = inputArea->toPlainText();

    if (!encryptorOutput.isEmpty())
    {
        Protocol::Error errorCode(engines[protocolSelecter->currentData().toString()]->cipher(encryptorOutput));


        if (errorCode == Protocol::Error::Aborted) { /* Operation cancelled, there is nothing to do */ }

        else if (errorCode == Protocol::Error::NoError)  // Operation is successful, we just have to display or save the output
            handleOutput();
        else
            QMessageBox::critical(this, tr("Runtime error #") + QString::number(errorCode), errors[Protocol::Error(errorCode)]);
    }
    else
        QMessageBox::warning(this, tr("Little problem..."), tr("Maybe you could type something before asking to translate..."));

}
void EncryptorWidget::decipher ()
{
    encryptorOutput = inputArea->toPlainText();

    if (!encryptorOutput.isEmpty())
    {
        uint8_t errorCode(engines[protocolSelecter->currentData().toString()]->decipher(encryptorOutput));


        if (errorCode == 42u) { /* Operation cancelled, there is nothing to do */ }

        else if (errorCode == 0u)  // Operation is successful, we just have to display or save the output
            handleOutput();
        else
            QMessageBox::critical(this, tr("Runtime error #") + QString::number(errorCode), errors[Protocol::Error(errorCode)]);
    }
    else
        QMessageBox::warning(this, tr("Little problem..."), tr("Maybe you could type something before asking to translate..."));
}


void EncryptorWidget::handleOutput ()
{
    TranslationReportDialog report(encryptorOutput);
    report.setMinimumSize(report.size());

    report.exec();
}
