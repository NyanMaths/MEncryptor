#include <QInputDialog>

#include "src/protocols/unicode-values.hpp"


UnicodeValues::UnicodeValues () : Protocol()
{
    // Nothing to be done here
}


Protocol::Error UnicodeValues::cipher (QString& text) const
{
    bool confirmedInput;
    int base(QInputDialog::getInt(mainWindow(), QObject::tr("Choose base"), QObject::tr("Please input the numerical base you want"), 10, 2, 36, 1, &confirmedInput));
    if (!confirmedInput)
        return Protocol::Error::Aborted;


    QVector<QString> encryptedText;

    for (qsizetype i(0) ; i != text.length() ; i++)
        encryptedText += QString::number(text.at(i).unicode(), base);


    text = encryptedText.join(" ");

    return Protocol::Error::NoError;
}

Protocol::Error UnicodeValues::decipher (QString& text) const
{
    bool confirmedInput;
    int base(QInputDialog::getInt(mainWindow(), QObject::tr("Choose base"), QObject::tr("Please input the numerical base you want"), 10, 2, 36, 1, &confirmedInput));
    if (!confirmedInput)
        return Protocol::Error::Aborted;


    QStringList decryptedText(text.split(' '));
    bool successfullConversion;

    for (qsizetype i(0) ; i != decryptedText.length() ; i++)
    {
        decryptedText[i] = QChar(decryptedText.at(i).toInt(&successfullConversion, base));

        if (!successfullConversion)
            return Protocol::Error::CorruptedMessage;
    }


    text = decryptedText.join("");

    return Protocol::Error::NoError;
}
