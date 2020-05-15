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

    loadOptions ();


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
    options.push_back ("0");
    options.push_back ("0");
    options.push_back (tr("Frequency (decroissant)").toStdString ());

    std::ifstream optionsStream ("Options.pastouche");

    if (optionsStream)
    {
        std::string value;

        for (unsigned short i = 0 ; i != 6 && getline (optionsStream, value) ; i++)
            options[i] = value;
    }
}

void MEncryptor::loadOptions ()
{
    languageSelecter->setCurrentText (QString::fromStdString (options.at (1)));
    protocolSelecter->setCurrentText (QString::fromStdString (options.at (2)));
    analyzeSymbols->setChecked (QString::fromStdString (options.at (3)).toUShort ());
    caseSensitive->setChecked (QString::fromStdString (options.at (4)).toUShort ());
    sortingSelecter->setCurrentText (QString::fromStdString (options.at (5)));


    connect (languageSelecter, SIGNAL (currentIndexChanged (int)), this, SLOT (languageModified (int)));
}


MEncryptor::~MEncryptor ()
{
    std::ofstream optionsStream ("Options.pastouche");

    if (optionsStream)
    {
        optionsStream<<languageSelecter->currentData ().toString ().toStdString ()<<"\n"
                     <<languageSelecter->currentText ().toStdString ()<<"\n"
                     <<protocolSelecter->currentText ().toStdString ()<<"\n"
                     <<QString::number (analyzeSymbols->isChecked ()).toStdString ()<<"\n"
                     <<QString::number (caseSensitive->isChecked ()).toStdString ()<<"\n"
                     <<sortingSelecter->currentText ().toStdString ();
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
    inputZone->setAcceptRichText (false);


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
    analyzerInputZone->setAcceptRichText (false);

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


    analyzerOutputBox = new QGroupBox (tr("Analysis report"));
    analyzerTabLayout->addWidget (analyzerOutputBox);
    analyzerOutputBoxLayout = new QGridLayout (analyzerOutputBox);

    analyzerOutputZone = new QTextEdit;
    analyzerOutputBoxLayout->addWidget (analyzerOutputZone, 0, 0, 1, 2);
    analyzerOutputZone->setReadOnly (true);

    chooseSortingLabel = new QLabel (tr("Sort by :"));
    analyzerOutputBoxLayout->addWidget (chooseSortingLabel, 1, 0, 1, 1, Qt::AlignRight);

    sortingSelecter = new QComboBox;
    sortingSelecter->addItem (tr("Alphabetical order"));
    sortingSelecter->addItem (tr("Frequency (croissant)"));
    sortingSelecter->addItem (tr("Frequency (decroissant)"));
    analyzerOutputBoxLayout->addWidget (sortingSelecter, 1, 1);

    connect (sortingSelecter, SIGNAL (currentTextChanged ()), this, SLOT (analyzeText ()));
}


//////////////  Slots


void MEncryptor::analyzeText ()
{
    QString input = analyzerInputZone->toPlainText ().remove (" ").remove ("\t").remove ("\n");

    if (!analyzerInputZone->toPlainText ().isEmpty ())
    {
        analyzerOutputZone->clear ();


        QString dict;

        if (!caseSensitive->isChecked ())
            input = input.toUpper ();


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

        displayAnalysisReport (input, dict);
    }
    else
        QMessageBox::warning (this, tr("Oooooooops..."), tr("You should enter something before analysis !"));
}

void MEncryptor::clearContents ()
{
    analyzerInputZone->clear ();
    analyzerOutputZone->clear ();
}


//////////////  Others


void MEncryptor::displayAnalysisReport (const QString& input, const QString& dict)
{
    QStringList splitDict (dict.split (""));
    splitDict.sort ();
    splitDict.removeAll ("");


    if (sortingSelecter->currentText () == tr("Alphabetical order"))
    {
        for (int i = 0 ; i != splitDict.length () ; i++)
            analyzerOutputZone->setText (analyzerOutputZone->toPlainText () + QString::number (input.count (splitDict.at (i))) + tr(" occurrence(s) of character ") + splitDict.at (i) + "\n");
    }
    else
    {
        if (sortingSelecter->currentText () == tr("Frequency (croissant)"))
            splitDict = reverse (splitDict);

        QStringList splitReport;
        unsigned int currentCharIndex;

        while (!splitDict.isEmpty ())
        {
            currentCharIndex = mostFrequent (splitDict, input);

            splitReport += QString::number (input.count (splitDict.at (currentCharIndex))) + tr(" occurrence(s) of character ") + splitDict.at (currentCharIndex);

            splitDict.removeAt (currentCharIndex);
        }

        if (sortingSelecter->currentText () == tr("Frequency (croissant)"))
            splitReport = reverse (splitReport);

        analyzerOutputZone->setText (splitReport.join ("\n"));
    }
}

unsigned int MEncryptor::mostFrequent (const QStringList& dict, const QString& input)
{
    QString mostFrequent = dict.at (0);

    for (int i = 1 ; i != dict.length () ; i++)
    {
        if (input.count (mostFrequent) < input.count (dict.at (i)))
            mostFrequent = dict.at (i);
    }

    return dict.indexOf (mostFrequent);
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
        encryptorOutput = inputZone->toPlainText ();
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
            destFile<<encryptorOutput.toStdString ();
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
        ouputPreview.setText (encryptorOutput);

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

    encryptorOutput = encryptorOutput.toLower ();

    for (unsigned short int i = 0 ; i != dict.length () ; i++)
        encryptorOutput = encryptorOutput.replace (dict.at (i), leetDict.at (i));

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

    encryptorOutput = encryptorOutput.replace ("m", "M");
    encryptorOutput = encryptorOutput.replace ("q", "Q");
    encryptorOutput = encryptorOutput.replace ("v", "V");

    for (unsigned short int i = 0 ; i != dict.length () ; i++)
        encryptorOutput = encryptorOutput.replace (leetDict.at (i), dict.at (i));

    return 0;
}


//////////////  Reversed Words


unsigned short int MEncryptor::reversedWords ()
{
    QStringList words = encryptorOutput.split (" ");

    for (int i = 0 ; i != words.length () ; i++)
        words[i] = reverse (words.at (i));

    encryptorOutput = words.join (" ");

    return 0;
}


//////////////  Reversed Sentence


unsigned short int MEncryptor::reversedSentence ()
{
    encryptorOutput = reverse (encryptorOutput);

    return 0;
}


//////////////  ASCII Values


unsigned short int MEncryptor::e_ASCIIValues ()
{
    QStringList finalOutput;

    for (int i = 0 ; i != encryptorOutput.length () ; i++)
        finalOutput.push_back (QString::number (encryptorOutput.at (i).unicode ()));

    encryptorOutput = finalOutput.join (" ");

    return 0;
}

unsigned short int MEncryptor::d_ASCIIValues ()
{
    if (encryptorOutput.contains (QRegExp ("[^\\d ]")))
        return 2;

    QStringList outputChars = encryptorOutput.split (" ");

    for (int i = 0 ; i != outputChars.length () ; i++)
        outputChars[i] = QChar (outputChars.at (i).toInt ());

    encryptorOutput = outputChars.join ("");

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
        for (int i = 0 ; i != encryptorOutput.length () ; i++)
        {
            for (int dictIndex = 0 ; dictIndex != 52 ; dictIndex++)
            {
                if (encryptorOutput.at (i) == dict.at (dictIndex))
                {
                    if (dictIndex + 2 * shift > 51)
                        encryptorOutput[i] = dict.at (dictIndex + 2 * shift - dict.length ());

                    else
                        encryptorOutput[i] = dict.at (dictIndex + 2 * shift);

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
        for (int i = 0 ; i != encryptorOutput.length () ; i++)
        {
            for (int dictIndex = 0 ; dictIndex != 52 ; dictIndex++)
            {
                if (encryptorOutput.at (i) == dict.at (dictIndex))
                {
                    if (dictIndex - 2 * shift < 0)
                        encryptorOutput[i] = dict.at (dictIndex - 2 * shift + dict.length ());

                    else
                        encryptorOutput[i] = dict.at (dictIndex - 2 * shift);

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


    encryptorOutput = encryptorOutput.toLower ();

    for (unsigned short int i = 0 ; i != dict.length () ; i++)
        encryptorOutput = encryptorOutput.replace (dict.at (i), brailleDict.at (i));

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
        encryptorOutput = encryptorOutput.replace (brailleDict.at (i), dict.at (i));

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
    encryptorOutput = encryptorOutput.toLower ();
    bool charInString;

    for (int i = 0 ; i != encryptorOutput.length () ; i++)
    {
        charInString = dict.contains (QString (encryptorOutput.at (i)));

        if (charInString && encryptorOutput.at (i + 1) != "\n")
            tmpOutput += QString::number (dict.indexOf (encryptorOutput.at (i))) + ' ';

        else if (charInString)
            tmpOutput += QString::number (dict.indexOf (encryptorOutput.at (i)));

        else
            tmpOutput += encryptorOutput.at (i) + ' ';
    }

    encryptorOutput = tmpOutput;
    encryptorOutput.resize (encryptorOutput.length () - 1);

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


    QStringList outputChars (encryptorOutput.split (" "));
    encryptorOutput.clear ();


    bool number;

    for (int i = 0 ; i != outputChars.length () ; i++)
    {
        outputChars.at (i).toInt (&number);

        if (number)
            encryptorOutput += dict.at (outputChars.at (i).toInt ());

        else
            encryptorOutput += outputChars.at (i);
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

QStringList MEncryptor::reverse (const QStringList& strList)
{
    QStringList reversedStrList;

    for (int i = strList.length () - 1 ; i != -1 ; i--)
        reversedStrList.push_back (strList.at (i));

    return reversedStrList;
}

