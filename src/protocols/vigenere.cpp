#include <QRegularExpression>

#include <QInputDialog>

#include "src/protocols/vigenere.hpp"


const QString Vigenere::dictionnary("aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ");

Vigenere::Vigenere () : Protocol()
{
    // Nothing to be done here
}


Protocol::Error Vigenere::cipher (QString& text) const
{
    bool ok;
    QString key(QInputDialog::getText(mainWindow(), QObject::tr("Vigenère cipher"), QObject::tr("Please input the key (Any non-alphabetic characters will be ignored)"), QLineEdit::Normal, "", &ok));

    if (key.isEmpty() || !ok)
        return Protocol::Error::Aborted;

    key = key.remove(QRegularExpression("^[a-zA-Z]"));


    qsizetype i(0), keyIndex, indexOfChar;

    while (i != text.length())
    {
        keyIndex = 0;

        while (keyIndex != key.length() && i != text.length())
        {
            indexOfChar = dictionnary.indexOf(text.at(i));

            if (indexOfChar != -1)
            {
                if (indexOfChar + dictionnary.indexOf(key.at(keyIndex)) >= dictionnary.length())
                   text[i] = dictionnary.at(indexOfChar + dictionnary.indexOf(key.at(keyIndex)) - dictionnary.length());

                else
                    text[i] = dictionnary.at(indexOfChar + dictionnary.indexOf(key.at(keyIndex)));

                keyIndex++;
            }

            i++;
        }
    }

    return Protocol::Error::NoError;
}

Protocol::Error Vigenere::decipher (QString& text) const
{
    bool ok;
    QString key(QInputDialog::getText(mainWindow(), QObject::tr("Vigenère cipher"), QObject::tr("Please input the key (Any non-alphabetic characters will be ignored)"), QLineEdit::Normal, "", &ok));

    if (key.isEmpty() || !ok)
        return Protocol::Error::Aborted;

    key = key.remove(QRegularExpression("^[a-zA-Z]"));


    qsizetype i(0), keyIndex, indexOfChar;

    while (i != text.length())
    {
        keyIndex = 0;

        while (keyIndex != key.length() && i != text.length())
        {
            indexOfChar = dictionnary.indexOf(text.at(i));

            if (indexOfChar != -1)
            {
                if (indexOfChar - dictionnary.indexOf(key.at(keyIndex)) < 0)
                   text[i] = dictionnary.at(indexOfChar - dictionnary.indexOf(key.at(keyIndex)) + dictionnary.length());

                else
                    text[i] = dictionnary.at(indexOfChar - dictionnary.indexOf(key.at(keyIndex)));

                keyIndex++;
            }

            i++;
        }
    }

    return Protocol::Error::NoError;
}
