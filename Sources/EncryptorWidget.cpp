#include <fstream>
#include <QFile>

#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>

#include "EncryptorWidget.h"


//////////////  Initialize widget


EncryptorWidget::EncryptorWidget () : QWidget ()
{
    errors += tr("Some files are missing in the app directory,\nreinstall it should fix the problem !");
    errors += tr("The message that you try to encrypt/decrypt\nis corrupted (Forbidden symbols, etc...) !");
    errors += tr("You entered an invalid key, please retry !");

    layout = new QHBoxLayout (this);


    inputBox = new QGroupBox ("Message");
    inputBox->setFixedSize (320, 350);
    inputBoxLayout = new QVBoxLayout (inputBox);

    inputZone = new QTextEdit;
    inputBoxLayout->addWidget (inputZone);
    inputZone->setAcceptRichText (false);


    initOptionsWidget ();


    outputBox = new QGroupBox (tr("Output"));
    outputBoxLayout = new QVBoxLayout (outputBox);

    bDisplayOutput = new QPushButton (tr("Translate now !"));
    bSaveOutput = new QPushButton (tr("Save translated message as..."));

    connect (bSaveOutput, SIGNAL (clicked ()), this, SLOT (startProcessAndSave ()));
    connect (bDisplayOutput, SIGNAL (clicked ()), this, SLOT (startProcessAndDisplay ()));

    outputBoxLayout->addWidget (bDisplayOutput);
    outputBoxLayout->addWidget (bSaveOutput);


    layout->addWidget (inputBox);
    layout->addWidget (optionsWidget);
    layout->addWidget (outputBox);


    loadOptions ();
}


void EncryptorWidget::initOptionsWidget ()
{
    optionsWidget = new QWidget;
    optionsLayout = new QVBoxLayout (optionsWidget);

    processImage = new QLabel;

    chooseProcessLabel = new QLabel (tr("Choose what to do :"));

    processSelecter = new QPushButton (QIcon ("Closed Locker.png"), tr("Encrypt"));
    connect (processSelecter, SIGNAL (clicked ()), this, SLOT (updateProcessText ()));

    initProtocols ();


    optionsLayout->addWidget (processImage);
    optionsLayout->addWidget (chooseProcessLabel);
    optionsLayout->addWidget (processSelecter);
    optionsLayout->addWidget (chooseProtocolLabel);
    optionsLayout->addWidget (protocolSelecter);
}

void EncryptorWidget::initProtocols ()
{
    std::string lang ("en");

    std::ifstream optionsFile ("UI Options.pastouche");
    if (optionsFile)
        getline (optionsFile, lang);


    chooseProtocolLabel = new QLabel (tr("Please select protocol :"));

    protocolSelecter = new QComboBox;


    std::ifstream protocolsList ("Protocols/Protocols " + lang + ".pastouche");
    std::string protocolName;

    for (unsigned int i = 1 ; getline (protocolsList, protocolName) ; i++)
        protocolSelecter->addItem (QString::fromStdString (protocolName), QVariant (i));
}


void EncryptorWidget::loadOptions ()
{
    QFile settingsFile ("Encryptor Options.pastouche");

    if (settingsFile.open (QIODevice::ReadOnly | QIODevice::Text))
    {
        QString themeIndex = "0";

        QFile themeFile ("UI Options.pastouche");
        if (themeFile.open (QIODevice::ReadOnly | QIODevice::Text))
            themeIndex = QString (themeFile.readAll ()).split ("\n").at (2);

        if (themeIndex == "2")
            processImage->setPixmap (QPixmap ("Encryptor image dark.png"));

        else
            processImage->setPixmap (QPixmap ("Encryptor image.png"));


        QStringList data = QString (settingsFile.readAll ()).split ("\n");

        if (data.at (0) == "0")
        {
            processSelecter->setText (tr("Encrypt"));
            processSelecter->setIcon (QIcon ("Closed Locker.png"));
        }
        else
        {
            processSelecter->setText (tr("Decrypt"));
            processSelecter->setIcon (QIcon ("Opened Locker.png"));
        }


        protocolSelecter->setCurrentIndex (data.at (1).toUShort ());
    }
    else
    {
        processImage->setPixmap (QPixmap ("Encryptor image.png"));

        processSelecter->setText (tr("Encrypt"));
        processSelecter->setIcon (QIcon ("Closed Locker.png"));

        protocolSelecter->setCurrentIndex (0);
    }
}


EncryptorWidget::~EncryptorWidget ()
{
    std::ofstream settingsFile ("Encryptor Options.pastouche");

    if (settingsFile)
    {
        if (processSelecter->text () == tr("Encrypt"))
            settingsFile<<0;

        else
            settingsFile<<1;

        settingsFile<<"\n"<<protocolSelecter->currentIndex ();
    }
}


//////////////  Slots


void EncryptorWidget::updateProcessText ()
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

void EncryptorWidget::updateOptionsImage (QString color)
{
    if (color == "dark")
        processImage->setPixmap (QPixmap ("Encryptor image dark.png"));

    else
        processImage->setPixmap (QPixmap ("Encryptor image.png"));
}


void EncryptorWidget::startProcessAndDisplay ()
{
    chooseProcess (false);
}
void EncryptorWidget::startProcessAndSave ()
{
    chooseProcess (true);
}

void EncryptorWidget::chooseProcess (bool save)
{
    if (!inputZone->toPlainText ().isEmpty ())
    {
        encryptorOutput = inputZone->toPlainText ();
        unsigned int selectedProtocol = protocolSelecter->currentData ().toUInt ();
        unsigned short int errorCode = 0;

        bool encrypt = true;
        if (processSelecter->text () == tr("Decrypt"))
            encrypt = false;


        if (selectedProtocol < 8)
            errorCode = protocolCallBack_1 (encrypt, selectedProtocol);

        else
            errorCode = protocolCallBack_2 (encrypt, selectedProtocol);


        if (errorCode == 42) { }

        else if (!errorCode)
            outputHandling (save);

        else
            QMessageBox::critical (this, tr("Runtime error #") + QString::number (errorCode), errors.at (errorCode - 1));
    }
    else
        QMessageBox::warning (this, tr("Little problem..."), tr("You should enter something in the text area to translate !"));
}

unsigned short int EncryptorWidget::protocolCallBack_1 (bool encrypt, unsigned int protocolCode)
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
            return e_UnicodeValues (10);

        return d_UnicodeValues (10);

    case 5:
        if (encrypt)
            return e_UnicodeValues (2);

        return d_UnicodeValues (2);

    case 6:
        if (encrypt)
            return e_UnicodeValues (8);

        return d_UnicodeValues (8);

    case 7:
        if (encrypt)
            return e_UnicodeValues (16);

        return d_UnicodeValues (16);

    default:
        break;
    }

    return 0;
}

unsigned short int EncryptorWidget::protocolCallBack_2 (bool encrypt, unsigned int protocolCode)
{
    switch (protocolCode)
    {
    case 8:
        if (encrypt)
            return e_Shift ();

        return d_Shift ();

    case 9:
        if (encrypt)
            return e_Braille ();

        return d_Braille ();

    case 10:
        if (encrypt)
            return e_Values ();

        return d_Values ();

    case 11:
        if (encrypt)
            return e_Vigenere ();

        return d_Vigenere ();

    case 12:
        if (encrypt)
            return e_Greek ();

        return d_Greek ();

    case 13:
        if (encrypt)
            return e_Cyrillic ();

        return d_Cyrillic ();

    case 14:
        if (encrypt)
            return e_Morse ();

        return d_Morse ();

    default:
        break;
    }

    return 0;
}


void EncryptorWidget::outputHandling (bool save)
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


unsigned short int EncryptorWidget::e_LeetSpeak ()
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

unsigned short int EncryptorWidget::d_LeetSpeak ()
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


unsigned short int EncryptorWidget::reversedWords ()
{
    QStringList words = encryptorOutput.split (" ");

    for (int i = 0 ; i != words.length () ; i++)
        words[i] = reverse (words.at (i));

    encryptorOutput = words.join (" ");

    return 0;
}


//////////////  Reversed Sentence


unsigned short int EncryptorWidget::reversedSentence ()
{
    encryptorOutput = reverse (encryptorOutput);

    return 0;
}


//////////////  Greek


unsigned short int EncryptorWidget::e_Greek ()
{
    QFile protocolDatFile ("Protocols/Greek/Dictionary.mt_data");
    if (!protocolDatFile.open (QIODevice::ReadOnly | QIODevice::Text))
        return 1;

    QString dict (protocolDatFile.readLine ()), greekDict (protocolDatFile.readLine ());


    for (unsigned short int i = 0 ; i != dict.length () ; i++)
        encryptorOutput = encryptorOutput.replace (dict.at (i), greekDict.at (i));

    return 0;
}

unsigned short int EncryptorWidget::d_Greek ()
{
    QFile protocolDatFile ("Protocols/Greek/Dictionary.mt_data");
    if (!protocolDatFile.open (QIODevice::ReadOnly | QIODevice::Text))
        return 1;

    QString dict (protocolDatFile.readLine ()), greekDict (protocolDatFile.readLine ());


    for (unsigned short int i = 0 ; i != dict.length () ; i++)
        encryptorOutput = encryptorOutput.replace (greekDict.at (i), dict.at (i));

    return 0;
}


//////////////  Cyrillic


unsigned short int EncryptorWidget::e_Cyrillic ()
{
    QFile protocolDatFile ("Protocols/Cyrillic/Dictionary.mt_data");
    if (!protocolDatFile.open (QIODevice::ReadOnly | QIODevice::Text))
        return 1;

    QString dict (protocolDatFile.readLine ()), cyrillicDict (protocolDatFile.readLine ());


    for (unsigned short int i = 0 ; i != dict.length () ; i++)
        encryptorOutput = encryptorOutput.replace (dict.at (i), cyrillicDict.at (i));

    return 0;
}

unsigned short int EncryptorWidget::d_Cyrillic ()
{
    QFile protocolDatFile ("Protocols/Cyrillic/Dictionary.mt_data");
    if (!protocolDatFile.open (QIODevice::ReadOnly | QIODevice::Text))
        return 1;

    QString dict (protocolDatFile.readLine ()), cyrillicDict (protocolDatFile.readLine ());


    for (unsigned short int i = 0 ; i != dict.length () ; i++)
        encryptorOutput = encryptorOutput.replace (cyrillicDict.at (i), dict.at (i));

    return 0;
}


//////////////  Unicode Values


unsigned short int EncryptorWidget::e_UnicodeValues (unsigned short int base)
{
    QStringList finalOutput;

    for (int i = 0 ; i != encryptorOutput.length () ; i++)
        finalOutput.push_back (QString::number (encryptorOutput.at (i).unicode (), base));

    encryptorOutput = finalOutput.join (" ");

    return 0;
}

unsigned short int EncryptorWidget::d_UnicodeValues (unsigned short int base)
{
    if (base == 2)
    {
        if (encryptorOutput.contains (QRegExp ("[^01 ]")))
            return 2;
    }
    else if (base == 8)
    {
        if (encryptorOutput.contains (QRegExp ("[^01234567 ]")))
            return 2;
    }
    else if (base == 10)
    {
        if (encryptorOutput.contains (QRegExp ("[^\\d ]")))
            return 2;
    }
    else
    {
        if (encryptorOutput.toUpper ().contains (QRegExp ("[^\\dABCEDF ]")))
            return 2;
    }

    QStringList outputChars = encryptorOutput.split (" ");

    for (int i = 0 ; i != outputChars.length () ; i++)
        outputChars[i] = QChar (outputChars.at (i).toInt (nullptr, base));

    encryptorOutput = outputChars.join ("");

    return 0;
}


//////////////  Shift


unsigned short int EncryptorWidget::e_Shift ()
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

unsigned short int EncryptorWidget::d_Shift ()
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


unsigned short int EncryptorWidget::e_Braille ()
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

unsigned short int EncryptorWidget::d_Braille ()
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


unsigned short int EncryptorWidget::e_Values ()
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

unsigned short int EncryptorWidget::d_Values ()
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


unsigned short int EncryptorWidget::e_Vigenere ()
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

unsigned short int EncryptorWidget::d_Vigenere ()
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


//////////////  Morse


unsigned short int EncryptorWidget::e_Morse ()
{
    QFile protocolDatStream ("Protocols/Morse/Dictionary.mt_data");

    if (!protocolDatStream.open (QIODevice::ReadOnly | QIODevice::Text))
        return 1;

    QString dict (QString (protocolDatStream.readLine ()));
    QStringList morseDict (QString (protocolDatStream.readLine ()).split (" "));
    dict.resize (dict.length () - 1);
    morseDict.removeAll ("\n");


    QStringList splitOutput = encryptorOutput.toLower ().replace (QRegExp (" +"), " ").split ("");

    for (int i = 0 ; i != dict.length () ; i++)
    {
        while (splitOutput.count (dict.at (i)))
            splitOutput.replace (splitOutput.indexOf (dict.at (i)), morseDict.at (i));
    }

    encryptorOutput = splitOutput.join ("/").remove (" ").remove (0, 1).replace ("/\n/", "//\n");

    return 0;
}

unsigned short int EncryptorWidget::d_Morse ()
{
    QFile protocolDatStream ("Protocols/Morse/Dictionary.mt_data");

    if (!protocolDatStream.open (QIODevice::ReadOnly | QIODevice::Text))
        return 1;

    QString dict (QString (protocolDatStream.readLine ()));
    QStringList morseDict (QString (protocolDatStream.readLine ()).split (" "));
    dict[dict.length () - 1] = ' ';
    morseDict.removeAll ("\n");
    morseDict.push_back ("");


    QStringList splitOutput = encryptorOutput.remove (" ").replace ("//\n", "/\n/").split ("/");

    for (int i = 0 ; i != dict.length () ; i++)
    {
        while (splitOutput.count (morseDict.at (i)))
            splitOutput.replace (splitOutput.indexOf (morseDict.at (i)), dict.at (i));
    }

    encryptorOutput = splitOutput.join ("");

    return 0;
}


////////////////////////////////////////  Tools


QString EncryptorWidget::reverse (const QString& str)
{
    QString reversedStr;

    for (int i = str.length () - 1 ; i != -1 ; i--)
        reversedStr.push_back (str.at (i));

    return reversedStr;
}
