#include <QApplication>
#include <QStyleFactory>

#include <fstream>

#include "MEncryptor.h"


////////////////////////////////////////  Constructor / Destructor


MEncryptor::MEncryptor () : QTabWidget ()
{
    QFontDatabase::addApplicationFont ("Ubuntu.ttf");
    qApp->setFont (QFont ("Ubuntu", 12));
    initPalettes ();
    readOptions ();

    errors += tr("Some files are missing in the app directory,\nreinstall it should fix the problem !");
    errors += tr("The message that you try to encrypt/decrypt\nis corrupted (Forbidden symbols, etc...) !");
    errors += tr("You entered an invalid key, please retry !");


    translator = new QTranslator;
    translator->load ("mencryptor_" + QString::fromStdString (options.at (0)));
    qApp->installTranslator (translator);

    messageBoxesTranslator = new QTranslator;
    messageBoxesTranslator->load ("qtbase_" + QString::fromStdString (options.at (0)));
    qApp->installTranslator (messageBoxesTranslator);


    initEncryptor (); // Initialize "Encryptor" tab
    initAnalyzer ();  // Initialize "Frequency Analyzer" tab
    initOthers ();    // Initialize "Help and Settings" tab


    loadOptions ();
    setWindowIcon (QIcon ("Window Icon.png"));

    show ();

    QSize screenSize = QGuiApplication::screens().at (0)->size ();
    move (screenSize.width () / 2 - width () / 2, screenSize.height () / 2 - height () / 2);

    setFixedSize (size ());
}

void MEncryptor::readOptions ()
{
    options.push_back ("en");
    options.push_back ("English");
    options.push_back ("0");  // Theme index
    options.push_back ("0");  // Process (0 == encrypt, 1 == decrypt)
    options.push_back ("0");  // Protocol index
    options.push_back ("0");  // Analyze symbols ? (boolean)
    options.push_back ("0");  // Case sensitive ? (boolean)
    options.push_back ("2");  // Sorting index

    std::ifstream optionsStream ("Options.pastouche");

    if (optionsStream)
    {
        std::string value;

        for (unsigned short i = 0 ; getline (optionsStream, value) ; i++)
            options[i] = value;
    }
}

void MEncryptor::initPalettes ()
{
    lightPalette = qApp->palette ();


    QColor darkColor = QColor (45, 45, 45);
    QColor disabledColor = QColor (127, 127, 127);

    darkPalette.setColor (QPalette::Window, darkColor);
    darkPalette.setColor (QPalette::WindowText, Qt::white);
    darkPalette.setColor (QPalette::Base, QColor (18, 18, 18));
    darkPalette.setColor (QPalette::AlternateBase, darkColor);
    darkPalette.setColor (QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor (QPalette::ToolTipText, Qt::white);
    darkPalette.setColor (QPalette::Text, Qt::white);
    darkPalette.setColor (QPalette::Disabled, QPalette::Text, disabledColor);
    darkPalette.setColor (QPalette::Button, darkColor);
    darkPalette.setColor (QPalette::ButtonText, Qt::white);
    darkPalette.setColor (QPalette::Disabled, QPalette::ButtonText, disabledColor);
    darkPalette.setColor (QPalette::BrightText, Qt::red);
    darkPalette.setColor (QPalette::Link, QColor (42, 130, 218));

    darkPalette.setColor (QPalette::Highlight, QColor (42, 130, 218));
    darkPalette.setColor (QPalette::HighlightedText, Qt::black);
    darkPalette.setColor (QPalette::Disabled, QPalette::HighlightedText, disabledColor);

    darkPalette.setColor (QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor (QPalette::ToolTipText, Qt::white);
}

void MEncryptor::initUITheme ()
{
    unsigned short int themeIndex = themeSelecter->currentIndex ();

    if (themeIndex == 0)
    {
        qApp->setStyle (QStyleFactory::create ("WindowsVista"));
        qApp->setPalette (lightPalette);

        processImage->setPixmap (QPixmap ("Encryptor image.png"));
    }
    else if (themeIndex == 1)
    {
        qApp->setStyle (QStyleFactory::create ("Fusion"));
        qApp->setPalette (lightPalette);

        processImage->setPixmap (QPixmap ("Encryptor image.png"));
    }
    else
    {
        qApp->setStyle (QStyleFactory::create ("Fusion"));
        qApp->setPalette (darkPalette);

        processImage->setPixmap (QPixmap ("Encryptor image dark.png"));
    }

    qApp->setFont (QFont ("Ubuntu", 12));
}

void MEncryptor::loadOptions ()
{
    if (options.at (3) == "0")
    {
        processSelecter->setText (tr("Encrypt"));
        processSelecter->setIcon (QIcon ("Closed Locker.png"));
    }
    else
    {
        processSelecter->setText (tr("Decrypt"));
        processSelecter->setIcon (QIcon ("Opened Locker.png"));
    }

    protocolSelecter->setCurrentIndex (QString::fromStdString (options.at (4)).toUShort ());

    analyzeSymbols->setChecked (QString::fromStdString (options.at (5)).toUShort ());
    caseSensitive->setChecked (QString::fromStdString (options.at (6)).toUShort ());
    sortingSelecter->setCurrentIndex (QString::fromStdString (options.at (7)).toUShort ());

    languageSelecter->setCurrentText (QString::fromStdString (options.at (1)));
    themeSelecter->setCurrentIndex (QString::fromStdString (options.at (2)).toUShort ());


    initUITheme ();


    connect (languageSelecter, SIGNAL (currentIndexChanged (int)), this, SLOT (languageModified ()));
    connect (sortingSelecter, SIGNAL (currentIndexChanged (int)), this, SLOT (analyzeText ()));
    connect (themeSelecter, SIGNAL (currentIndexChanged (int)), this, SLOT (initUITheme ()));
}


MEncryptor::~MEncryptor ()
{
    std::ofstream optionsStream ("Options.pastouche");

    if (optionsStream)
    {
        optionsStream<<languageSelecter->currentData ().toString ().toStdString ()<<"\n"
                     <<languageSelecter->currentText ().toStdString ()<<"\n"
                     <<themeSelecter->currentIndex ()<<"\n";

        if (processSelecter->text () == tr("Encrypt"))
            optionsStream<<0;
        else
            optionsStream<<1;

        optionsStream<<"\n"<<protocolSelecter->currentIndex ()<<"\n"
                     <<analyzeSymbols->isChecked ()<<"\n"
                     <<caseSensitive->isChecked ()<<"\n"
                     <<sortingSelecter->currentIndex ()<<"\n";
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


////////////////////////////////////////  "Help and settings" tab


void MEncryptor::initOthers ()
{
    othersTab = new QWidget;
    addTab (othersTab, tr("Help and settings"));
    othersTabLayout = new QGridLayout (othersTab);


    aboutMRecorderLabel = new QLabel (tr(
       "This opensource software was developed by NY4N_M4THS (MemeTech INC)<br/>"
       "under <a href = 'https://gnu.org/licenses/lgpl-3.0.en.html'>license LGPLv3</a> with the framework Qt in C++ language.<br/>"
       "Follow <a href = 'https://qt.io'>this link</a> to learn more about Qt.<br/>"
       "You can also visit <a href = 'https://memetech-inc.weebly.com'>our website</a> to check for updates,<br/>"
       "try other of our applications or <a href = 'https://github.com/NyanMaths/MEncryptor/issues'>ask for new features</a> !<br/>"
       "Click <a href = 'https://github.com/NyanMaths/MEncryptor'>here</a> to visit the GitHub page of the project."));

    aboutMRecorderLabel->setOpenExternalLinks (true);
    aboutMRecorderLabel->setTextInteractionFlags (Qt::TextBrowserInteraction);
    aboutMRecorderLabel->setTextFormat (Qt::RichText);


    UIOptionsBox = new QGroupBox (tr("UI settings"));
    UIOptionsBoxLayout = new QGridLayout (UIOptionsBox);
    UIOptionsBoxLayout->setAlignment (Qt::AlignLeft);

    chooseLanguageLabel = new QLabel (tr("Display language :"));
    languageSelecter = new QComboBox;
    languageSelecter->addItem ("English", QVariant ("en"));
    languageSelecter->addItem ("Français", QVariant ("fr"));

    chooseThemeLabel = new QLabel (tr("Interface theme :"));
    themeSelecter = new QComboBox;
    themeSelecter->addItem (tr("Classic light"));
    themeSelecter->addItem (tr("Fusion light"));
    themeSelecter->addItem (tr("Fusion dark"));


    UIOptionsBoxLayout->addWidget (chooseLanguageLabel, 0, 0);
    UIOptionsBoxLayout->addWidget (languageSelecter, 0, 1);
    UIOptionsBoxLayout->addWidget (chooseThemeLabel, 1, 0);
    UIOptionsBoxLayout->addWidget (themeSelecter, 1, 1);


    othersTabLayout->addWidget (aboutMRecorderLabel, 0, 0, 1, 2);
    othersTabLayout->addWidget (UIOptionsBox, 1, 0);
}


//////////////  Slots


void MEncryptor::languageModified ()
{
    QMessageBox::Button clickedButton = QMessageBox::question (this, tr("Language changed"), tr("You need to restart the app to apply changes.\nDo you want to restart the app now ?"));

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
            errorCode = protocolCallBack_1 (encrypt, selectedProtocol);


        if (errorCode == 42) { }

        else if (!errorCode)
            outputHandling (save);

        else
            QMessageBox::critical (this, tr("Runtime error #") + QString::number (errorCode), errors.at (errorCode - 1));
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
            return e_UnicodeValues ();

        return d_UnicodeValues ();

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

    case 8:
        if (encrypt)
            return e_Vigenere ();

        return d_Vigenere ();

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


//////////////  Unicode Values


unsigned short int MEncryptor::e_UnicodeValues ()
{
    QStringList finalOutput;

    for (int i = 0 ; i != encryptorOutput.length () ; i++)
        finalOutput.push_back (QString::number (encryptorOutput.at (i).unicode ()));

    encryptorOutput = finalOutput.join (" ");

    return 0;
}

unsigned short int MEncryptor::d_UnicodeValues ()
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


    QString tmpOutput;
    encryptorOutput = encryptorOutput.toLower ();
    bool charInDict;

    for (int i = 0 ; i != encryptorOutput.length () ; i++)
    {
        charInDict = dict.contains (QString (encryptorOutput.at (i)));

        if (charInDict && encryptorOutput.at (i + 1) != "\n")
            tmpOutput += QString::number (dict.indexOf (encryptorOutput.at (i))) + ' ';

        else if (charInDict)
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

        if (number && outputChars.at (i).toInt () < dict.length () && outputChars.at (i).toInt () > -1)
            encryptorOutput += dict.at (outputChars.at (i).toInt ());

        else if (outputChars.at (i).length () == 1)
            encryptorOutput += outputChars.at (i);

        else
            return 2;
    }

    return 0;
}


//////////////  Vigenère


unsigned short int MEncryptor::e_Vigenere ()
{
    bool ok;
    QString dict ("aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ");


    QString enteredKey = QInputDialog::getText(this, tr("Vigenère cipher"), tr("Please input the key (Alphabetic) :"), QLineEdit::Normal, "", &ok);

    if (enteredKey.isEmpty ())
        return 3;

    QString key;
    for (int i = 0 ; i != enteredKey.length () ; i++)
        if (dict.contains (enteredKey.at (i)))
            key += enteredKey.at (i);


    if (ok)
    {
        int messageIndex = 0, keyIndex = 0;

        while (messageIndex != encryptorOutput.length ())
        {
            keyIndex = 0;

            while (keyIndex != key.length () && messageIndex != encryptorOutput.length ())
            {
                if (dict.contains (encryptorOutput.at (messageIndex)))
                {
                    if (dict.indexOf (encryptorOutput.at (messageIndex)) + dict.indexOf (key.at (keyIndex)) >= dict.length ())
                       encryptorOutput[messageIndex] = dict.at (dict.indexOf (encryptorOutput.at (messageIndex)) + dict.indexOf (key.at (keyIndex)) - dict.length ());

                    else
                        encryptorOutput[messageIndex] = dict.at (dict.indexOf (encryptorOutput.at (messageIndex)) + dict.indexOf (key.at (keyIndex)));

                    keyIndex++;
                }

                messageIndex++;
            }
        }
    }
    else
        return 42;

    return 0;
}

unsigned short int MEncryptor::d_Vigenere ()
{
    bool ok;
    QString dict ("aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ");


    QString enteredKey = QInputDialog::getText(this, tr("Vigenère cipher"), tr("Please input the key (Alphabetic) :"), QLineEdit::Normal, "", &ok);

    if (enteredKey.isEmpty ())
        return 3;

    QString key;
    for (int i = 0 ; i != enteredKey.length () ; i++)
        if (dict.contains (enteredKey.at (i)))
            key += enteredKey.at (i);


    if (ok)
    {
        int messageIndex = 0, keyIndex = 0;

        while (messageIndex != encryptorOutput.length ())
        {
            keyIndex = 0;

            while (keyIndex != key.length () && messageIndex != encryptorOutput.length ())
            {
                if (dict.contains (encryptorOutput.at (messageIndex)))
                {
                    if (dict.indexOf (encryptorOutput.at (messageIndex)) - dict.indexOf (key.at (keyIndex)) < 0)
                       encryptorOutput[messageIndex] = dict.at (dict.indexOf (encryptorOutput.at (messageIndex)) - dict.indexOf (key.at (keyIndex)) + dict.length ());

                    else
                        encryptorOutput[messageIndex] = dict.at (dict.indexOf (encryptorOutput.at (messageIndex)) - dict.indexOf (key.at (keyIndex)));

                    keyIndex++;
                }

                messageIndex++;
            }
        }
    }
    else
        return 42;

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

