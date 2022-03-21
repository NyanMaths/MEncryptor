#include <QRegularExpression>

#include "src/protocols/values.hpp"


const QString Values::decryptedAlphabet(" abcdefghijklmnopqrstuvwxyz");
const QVector<QString> Values::encryptedAlphabet
{
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "10",
    "11",
    "12",
    "13",
    "14",
    "15",
    "16",
    "17",
    "18",
    "19",
    "20",
    "21",
    "22",
    "23",
    "24",
    "25",
    "26"
};

Values::Values () : Protocol()
{
    // Nothing to be done here
}


Protocol::Error Values::cipher (QString& text) const
{
    text = text.toLower();
    QVector<QString> encryptedText;

    for (qsizetype i(0) ; i != text.length() ; i++)
    {
        if (decryptedAlphabet.contains(text.at(i)))
            encryptedText += encryptedAlphabet.at(decryptedAlphabet.indexOf(text.at(i)));

        else
            encryptedText += text.at(i);
    }

    text = encryptedText.join(" ").replace(QRegularExpression(" ?\n ?"), "\n");

    return Protocol::Error::NoError;
}

Protocol::Error Values::decipher (QString& text) const
{
    QVector<QString> decryptedText(text.replace("\n", " \n ").split(' '));

    for (qsizetype i(0) ; i != decryptedText.length() ; i++)
    {
        if (encryptedAlphabet.contains(decryptedText.at(i)))
            decryptedText[i] = decryptedAlphabet.at(encryptedAlphabet.indexOf(decryptedText.at(i)));

        else
            decryptedText[i] = decryptedText.at(i);
    }

    text = decryptedText.join("");

    return Protocol::Error::NoError;
}
