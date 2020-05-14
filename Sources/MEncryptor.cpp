#include <QApplication>

#include <fstream>

#include "MEncryptor.h"


////////////////////////////////////////  Constructor / Destructor


MEncryptor::MEncryptor () : QTabWidget ()
{
    setWindowIcon (QIcon ("Window Icon.png"));


    QFontDatabase::addApplicationFont ("Ubuntu.ttf");


    initOptions ();


    translator = new QTranslator;
    translator->load("mencryptor_" + QString::fromStdString (options.at (0)));
    qApp->installTranslator (translator);

    messageBoxesTranslator = new QTranslator;
    messageBoxesTranslator->load ("qtbase_" + QString::fromStdString (options.at (0)));
    qApp->installTranslator (messageBoxesTranslator);


    initEncryptor (); // Initialize "Encryptor" tab
    initAnalyzer ();  // Initialize "Frequency Analyzer" tab
    initOthers ();    // Initialize "Help and Settings" tab


    qApp->setFont (QFont ("Ubuntu", 12));

    show ();

    QSize screenSize = QGuiApplication::screens().at (0)->size ();
    move (screenSize.width () / 2 - width () / 2, screenSize.height () / 2 - height () / 2);

    setFixedSize (size ());
}

void MEncryptor::initOptions ()
{
    options.push_back ("en");
    options.push_back ("Language : English");
    options.push_back ("Leet Speak (1337)");

    std::ifstream optionsStream ("Options.pastouche");

    if (optionsStream)
    {
        std::string value;

        for (unsigned short i = 0 ; i != 3 && getline (optionsStream, value) ; i++)
            options[i] = value;
    }
}


MEncryptor::~MEncryptor ()
{
    std::ofstream optionsStream ("Options.pastouche");

    if (optionsStream)
    {
        optionsStream<<languageSelecter->currentData ().toString ().toStdString ()<<"\n"
                     <<languageSelecter->currentText ().toStdString ()<<"\n"
                     <<protocolSelecter->currentText ().toStdString ();
    }
}


////////////////////////////////////////  Encryptor tab


void MEncryptor::initEncryptor ()
{
    encryptorTab = new QWidget;
    addTab (encryptorTab, tr("Encrypt / Decrypt messages"));


    inputBox = new QGroupBox ("Message");
    inputBox->setFixedSize (320, 350);
    inputBoxLayout = new QVBoxLayout (inputBox);

    inputZone = new QTextEdit;
    inputBoxLayout->addWidget (inputZone);


    processWidget = new QWidget;
    processLayout = new QVBoxLayout (processWidget);

    processImage = new QLabel;
    processImage->setPixmap (QPixmap ("Process settings.png"));


    chooseProcessLabel = new QLabel (tr("Choose what to do :"));

    processSelecter = new QPushButton (QIcon ("Closed Locker.png"), tr("Encrypt"));
    connect (processSelecter, SIGNAL (clicked ()), this, SLOT (setProcessText ()));


    initProtocols ();


    processLayout->addWidget (processImage);
    processLayout->addWidget (chooseProcessLabel);
    processLayout->addWidget (processSelecter);
    processLayout->addWidget (chooseProtocolLabel);
    processLayout->addWidget (protocolSelecter);


    outputBox = new QGroupBox (tr("Output"));
    outputBoxLayout = new QVBoxLayout (outputBox);

    bDisplayOutput = new QPushButton (tr("Translate now !"));
    connect (bDisplayOutput, SIGNAL (clicked ()), this, SLOT (startProcessAndDisplay ()));

    bSaveOutput = new QPushButton (tr("Save translated message as..."));
    connect (bSaveOutput, SIGNAL (clicked ()), this, SLOT (startProcessAndSave ()));

    outputBoxLayout->addWidget (bDisplayOutput);
    outputBoxLayout->addWidget (bSaveOutput);


    encryptorLayout = new QHBoxLayout;
    encryptorTab->setLayout (encryptorLayout);
    encryptorLayout->addWidget (inputBox);
    encryptorLayout->addWidget (processWidget);
    encryptorLayout->addWidget (outputBox);
}

void MEncryptor::initProtocols ()
{
    chooseProtocolLabel = new QLabel (tr("Please select protocol :"));

    protocolSelecter = new QComboBox;


    std::ifstream protocolsList (std::string ("Protocols/Protocols ") + std::string (options.at (0)) + ".pastouche");
    std::string protocolName;

    for (unsigned int i = 1 ; getline (protocolsList, protocolName) ; i++)
        protocolSelecter->addItem (QString::fromStdString (protocolName), QVariant (i));


    protocolSelecter->setCurrentText (QString::fromStdString (options.at (2)));
}


//////////////  Slots


void MEncryptor::setProcessText ()
{
    if (processSelecter->text () == tr("Encrypt"))
    {
        processSelecter->setIcon (QIcon ("Opened Locker.png"));
        processSelecter->setText (tr("Decrypt"));
    }
    else
    {
        processSelecter->setIcon (QIcon ("Closed Locker.png"));
        processSelecter->setText (tr("Encrypt"));
    }
}


////////////////////////////////////////  Analyzer tab


void MEncryptor::initAnalyzer ()
{
    analyzerTab = new QWidget;
    addTab (analyzerTab, tr("Frequency analyzer"));
    analyzerTabLayout = new QHBoxLayout (analyzerTab);

    analyzerInputBox = new QGroupBox (tr("Message"));
    analyzerTabLayout->addWidget (analyzerInputBox);
    analyzerInputBoxLayout = new QGridLayout (analyzerInputBox);


    analyzerInputZone = new QTextEdit;
    analyzerInputBoxLayout->addWidget (analyzerInputZone, 0, 0, 1, 2);

    analyzeSymbols = new QCheckBox (tr("Also analyze symbols"));
    analyzerInputBoxLayout->addWidget (analyzeSymbols, 1, 0);

    caseSensitive = new QCheckBox (tr("Case sensitive"));
    analyzerInputBoxLayout->addWidget (caseSensitive, 2, 0);

    bAnalyze = new QPushButton (tr("Analyze"));
    analyzerInputBoxLayout->addWidget (bAnalyze, 1, 1);
    connect (bAnalyze, SIGNAL (clicked ()), this, SLOT (analyzeText ()));

    bClear = new QPushButton (tr("Clear"));
    analyzerInputBoxLayout->addWidget (bClear, 2, 1);
    connect (bClear, SIGNAL (clicked ()), this, SLOT (clearContents ()));


    analyzerOutputBox = new QGroupBox (tr("Analyze report"));
    analyzerTabLayout->addWidget (analyzerOutputBox);
    analyzerOutputBoxLayout = new QVBoxLayout (analyzerOutputBox);

    analyzerOutputZone = new QTextEdit;
    analyzerOutputBoxLayout->addWidget (analyzerOutputZone);
    analyzerOutputZone->setReadOnly (true);
}


void MEncryptor::analyzeText ()
{
    analyzerOutputZone->clear ();

    QString input (analyzerInputZone->toPlainText ());
    if (!caseSensitive->isChecked ())
        input = input.toUpper ();


    input = input.remove (" ").remove ("\t").remove ("\n");

    QString dict;

    if (analyzeSymbols->isChecked ())
    {
        for (int i = 0 ; i != input.length () ; i++)
        {
            if (!dict.contains (input.at (i)))
                dict += input.at (i);
        }
    }
    else
    {
        for (int i = 0 ; i != input.length () ; i++)
        {
            if (input.at (i).isLetter () && !dict.contains (input.at (i)))
                dict += input.at (i);
        }
    }

    QStringList splitDict (dict.split (""));
    splitDict.sort ();
    splitDict.removeAll ("");

    for (int i = 0 ; i != splitDict.length () ; i++)
        analyzerOutputZone->setText (analyzerOutputZone->toPlainText () + tr("Found %n occurence(s) of ", "", input.count (splitDict.at (i))) + splitDict.at (i) + "\n");
}

void MEncryptor::clearContents ()
{
    analyzerInputZone->clear ();
    analyzerOutputZone->clear ();
}


////////////////////////////////////////  Help and settings tab


void MEncryptor::initOthers ()
{
    othersTab = new QWidget;
    addTab (othersTab, tr("Help and settings"));
    othersTabLayout = new QVBoxLayout (othersTab);


    aboutMRecorderLabel = new QLabel (tr("This opensource software was developed by NY4N_M4THS (MemeTech INC)<br/>"
                                         "under <a href = 'https://gnu.org/licenses/lgpl-3.0.en.html'>license LGPLv3</a> with the framework Qt in C++ language.<br/>"
                                         "Follow <a href = 'https://qt.io'>this link</a> to learn more about Qt.<br/>"
                                         "You can also visit <a href = 'https://memetech-inc.weebly.com'>our website</a> to check for updates,<br/>"
                                         "try other of our applications or ask for new features !<br/>"
                                         "Click <a href = 'https://github.com/NyanMaths/MEncryptor'>here</a> to visit the GitHub page of the project.<br/><br/>"));
    QFile errorsStream (QString ("Error Codes ") + QString::fromStdString (options.at (0)) + ".pastouche");
    if (errorsStream.open (QFile::ReadOnly))
        aboutMRecorderLabel->setText (aboutMRecorderLabel->text () + errorsStream.readAll ());

    aboutMRecorderLabel->setOpenExternalLinks (true);
    aboutMRecorderLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    aboutMRecorderLabel->setTextFormat(Qt::RichText);


    languageSelecter = new QComboBox;

    languageSelecter->addItem ("Language : English", QVariant ("en"));
    languageSelecter->addItem ("Langue : Français", QVariant ("fr"));

    languageSelecter->setCurrentText (QString::fromStdString (options.at (1)));

    connect (languageSelecter, SIGNAL (currentIndexChanged (int)), this, SLOT (languageModified (int)));


    othersTabLayout->addWidget (aboutMRecorderLabel);
    othersTabLayout->addWidget (languageSelecter);
}


//////////////  Slots


void MEncryptor::languageModified (int)
{
    QMessageBox::Button clickedButton = QMessageBox::question (this, tr("Language changed"), tr("Do you want to restart the app now ?"));

    if (clickedButton == QMessageBox::Yes)
    {
        qApp->quit ();
        QProcess::startDetached (qApp->arguments ()[0], qApp->arguments ());
    }
}


////////////////////////////////////////  Encryptor process manager


void MEncryptor::startProcessAndDisplay ()
{
    chooseProcess (false);
}
void MEncryptor::startProcessAndSave ()
{
    chooseProcess (true);
}

void MEncryptor::chooseProcess (bool save)
{
    if (inputZone->toPlainText () != "")
    {
        output = inputZone->toPlainText ();
        unsigned int selectedProtocol = protocolSelecter->currentData ().toUInt ();
        unsigned short int errorCode = 0;

        bool encrypt = true;
        if (processSelecter->text () == tr("Decrypt"))
            encrypt = false;


        if (selectedProtocol < 11)
            protocolCallBack_1 (encrypt, selectedProtocol);


        if (errorCode == 42) { }

        else if (!errorCode)
            outputHandling (save);

        else
            QMessageBox::critical (this, tr("Runtime error"), tr("Error #") + QString::number (errorCode) + tr("\nCheck in \"Help and settings\" tab to fix it !"));
    }
    else
        QMessageBox::warning (this, tr("Little problem..."), tr("You should enter something in the text area to translate !"));
}

unsigned short int MEncryptor::protocolCallBack_1 (bool encrypt, unsigned int protocolCode)
{
    switch (protocolCode)
    {
    case 1:
        if (encrypt)
            return e_LeetSpeak ();

        return d_LeetSpeak ();

    case 2:
        return reversedWords ();

    case 3:
        return reversedSentence ();

    case 4:
        if (encrypt)
            return e_ASCIIValues ();

        return d_ASCIIValues ();

    case 5:
        if (encrypt)
            return e_Shift ();

        return d_Shift ();

    case 6:
        if (encrypt)
            return e_Braille ();

        return d_Braille ();

    case 7:
        if (encrypt)
            return e_Values ();

        return d_Values ();

    default:
        break;
    }

    return 65535;
}


void MEncryptor::outputHandling (bool save)
{
    if (save)
    {
        QString destFileName = QFileDialog::getSaveFileName (this, tr("Save translated file as..."), tr("Untitled.txt"));

        std::ofstream destFile (destFileName.toStdString ());

        if (destFileName != "")
        {
            destFile<<output.toStdString ();
            QMessageBox::information (this, tr("Operation successful, Houston !"), tr("File ") + destFileName + tr(" saved !"));
        }
    }
    else
    {
        QDialog outputPopup (this);
        outputPopup.setModal (true);
        QHBoxLayout popupLayout (&outputPopup);

        QTextEdit ouputPreview (&outputPopup);
        ouputPreview.setMinimumSize (450, 375);
        ouputPreview.setReadOnly (true);
        ouputPreview.setText (output);

        popupLayout.addWidget (&ouputPreview);

        outputPopup.exec ();
    }
}


////////////////////////////////////////  Encrypting/Decrypting algorithms


//////////////  Leet Speak


unsigned short int MEncryptor::e_LeetSpeak ()
{
    std::ifstream protocolDatStream ("Protocols/Leet Speak (1337)/Dictionary.mt_data");

    if (!protocolDatStream)
        return 1;

    std::string sDict, sLeetDict;
    getline (protocolDatStream, sDict);
    getline (protocolDatStream, sLeetDict);

    QString dict = QString::fromStdString (sDict);
    QString leetDict = QString::fromStdString (sLeetDict);

    output = output.toLower ();

    for (unsigned short int i = 0 ; i != dict.length () ; i++)
        output = output.replace (dict.at (i), leetDict.at (i));

    return 0;
}

unsigned short int MEncryptor::d_LeetSpeak ()
{
    std::ifstream protocolDatStream ("Protocols/Leet Speak (1337)/Dictionary.mt_data");

    if (!protocolDatStream)
        return 1;

    std::string sDict, sLeetDict;
    getline (protocolDatStream, sDict);
    getline (protocolDatStream, sLeetDict);

    QString dict = QString::fromStdString (sDict);
    QString leetDict = QString::fromStdString (sLeetDict);

    output = output.replace ("m", "M");
    output = output.replace ("q", "Q");
    output = output.replace ("v", "V");

    for (unsigned short int i = 0 ; i != dict.length () ; i++)
        output = output.replace (leetDict.at (i), dict.at (i));

    return 0;
}


//////////////  Reversed Words


unsigned short int MEncryptor::reversedWords ()
{
    QStringList words = output.split (" ");

    for (int i = 0 ; i != words.length () ; i++)
        words[i] = reverse (words.at (i));

    output = words.join (" ");

    return 0;
}


//////////////  Reversed Sentence


unsigned short int MEncryptor::reversedSentence ()
{
    output = reverse (output);

    return 0;
}


//////////////  ASCII Values


unsigned short int MEncryptor::e_ASCIIValues ()
{
    QStringList finalOutput;

    for (int i = 0 ; i != output.length () ; i++)
        finalOutput.push_back (QString::number (output.at (i).unicode ()));

    output = finalOutput.join (" ");

    return 0;
}

unsigned short int MEncryptor::d_ASCIIValues ()
{
    if (output.contains (QRegExp ("[^\\d ]")))
        return 2;

    QStringList outputChars = output.split (" ");

    for (int i = 0 ; i != outputChars.length () ; i++)
        outputChars[i] = QChar (outputChars.at (i).toInt ());

    output = outputChars.join ("");

    return 0;
}


//////////////  Shift


unsigned short int MEncryptor::e_Shift ()
{
    bool ok;
    QString dict ("aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ");

    int shift = QInputDialog::getInt (this, tr("Shift cipher"), tr("Please input the shift :"), 1, 1, 25, 1, &ok);


    if (ok)
    {
        for (int i = 0 ; i != output.length () ; i++)
        {
            for (int dictIndex = 0 ; dictIndex != 52 ; dictIndex++)
            {
                if (output.at (i) == dict.at (dictIndex))
                {
                    if (dictIndex + 2 * shift > 51)
                        output[i] = dict.at (dictIndex + 2 * shift - dict.length ());

                    else
                        output[i] = dict.at (dictIndex + 2 * shift);

                    break;
                }
            }
        }
    }
    else
        return 42;

    return 0;
}

unsigned short int MEncryptor::d_Shift ()
{
    bool ok;
    QString dict ("aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ");

    int shift = QInputDialog::getInt (this, tr("Shift decipher"), tr("Please input the shift :"), 1, 1, 25, 1, &ok);


    if (ok)
    {
        for (int i = 0 ; i != output.length () ; i++)
        {
            for (int dictIndex = 0 ; dictIndex != 52 ; dictIndex++)
            {
                if (output.at (i) == dict.at (dictIndex))
                {
                    if (dictIndex - 2 * shift < 0)
                        output[i] = dict.at (dictIndex - 2 * shift + dict.length ());

                    else
                        output[i] = dict.at (dictIndex - 2 * shift);

                    break;
                }
            }
        }
    }
    else
        return 42;

    return 0;
}


//////////////  Braille


unsigned short int MEncryptor::e_Braille ()
{
    std::ifstream protocolDatStream ("Protocols/Braille/Dictionary.mt_data");

    if (!protocolDatStream)
        return 1;

    std::string sDict, sBrailleDict;
    getline (protocolDatStream, sDict);
    getline (protocolDatStream, sBrailleDict);

    QString dict = QString::fromStdString (sDict);
    QStringList brailleDict = QString::fromStdString (sBrailleDict).split (" ");


    output = output.toLower ();

    for (unsigned short int i = 0 ; i != dict.length () ; i++)
        output = output.replace (dict.at (i), brailleDict.at (i));

    return 0;
}

unsigned short int MEncryptor::d_Braille ()
{
    std::ifstream protocolDatStream ("Protocols/Braille/Dictionary.mt_data");

    if (!protocolDatStream)
        return 1;

    std::string sDict, sBrailleDict;
    getline (protocolDatStream, sDict);
    getline (protocolDatStream, sBrailleDict);

    QString dict = QString::fromStdString (sDict);
    QStringList brailleDict = QString::fromStdString (sBrailleDict).split (" ");


    for (unsigned short int i = 0 ; i != dict.length () ; i++)
        output = output.replace (brailleDict.at (i), dict.at (i));

    return 0;
}


//////////////  Values (A1Z26)


unsigned short int MEncryptor::e_Values ()
{
    std::ifstream protocolDatStream ("Protocols/Values (A1Z26)/Dictionary.mt_data");

    if (!protocolDatStream)
        return 1;

    std::string sDict;
    getline (protocolDatStream, sDict);
    QString dict (QString::fromStdString (sDict));


    QString tmpOutput ("");
    output = output.toLower ();
    bool charInString;

    for (int i = 0 ; i != output.length () ; i++)
    {
        charInString = dict.contains (QString (output.at (i)));

        if (charInString && output.at (i + 1) != "\n")
            tmpOutput += QString::number (dict.indexOf (output.at (i))) + ' ';

        else if (charInString)
            tmpOutput += QString::number (dict.indexOf (output.at (i)));

        else
            tmpOutput += output.at (i) + ' ';
    }

    output = tmpOutput;
    output.resize (output.length () - 1);

    return 0;
}

unsigned short int MEncryptor::d_Values ()
{
    std::ifstream protocolDatStream ("Protocols/Values (A1Z26)/Dictionary.mt_data");

    if (!protocolDatStream)
        return 1;

    std::string sDict;
    getline (protocolDatStream, sDict);
    QString dict (QString::fromStdString (sDict));


    QStringList outputChars (output.split (" "));
    output.clear ();


    bool number;

    for (int i = 0 ; i != outputChars.length () ; i++)
    {
        outputChars.at (i).toInt (&number);

        if (number)
            output += dict.at (outputChars.at (i).toInt ());

        else
            output += outputChars.at (i);
    }

    return 0;
}



////////////////////////////////////////  Tools


QString MEncryptor::reverse (const QString& str)
{
    QString reversedStr;

    for (int i = str.length () - 1 ; i != -1 ; i--)
        reversedStr.push_back (str.at (i));

    return reversedStr;
}

