#include <QInputDialog>
#include <QApplication>

#include "src/protocols/shift.hpp"


const QString Shift::dictionnary("aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ");

Shift::Shift () : Protocol()
{
    // Nothing to be done here
}


Protocol::Error Shift::cipher (QString& text) const
{
    bool ok;
    int shift(QInputDialog::getInt(mainWindow(), QObject::tr("Shift cipher"), QObject::tr("Please input the shift to apply"), 1, 1, 25, 1, &ok));

    if (!ok)
        return Protocol::Error::Aborted;


    for (qsizetype i(0) ; i != text.length() ; i++)
    {
        qsizetype indexOfChar(dictionnary.indexOf(text.at(i)));

        if (indexOfChar != -1)
            text[i] = dictionnary.at((indexOfChar + 2 * shift) % dictionnary.length());
    }

    return Protocol::Error::NoError;
}

Protocol::Error Shift::decipher (QString& text) const
{
    bool ok;
    int shift(QInputDialog::getInt(mainWindow(), QObject::tr("Shift cipher"), QObject::tr("Please input the shift previously applied"), 1, 1, 25, 1, &ok));

    if (!ok)
        return Protocol::Error::Aborted;


    for (qsizetype i(0) ; i != text.length() ; i++)
    {
        if (dictionnary.contains(text.at(i)))
        {
            qsizetype indexOfChar(dictionnary.indexOf(text.at(i)));

            if (indexOfChar != -1)
            {
                if (indexOfChar - 2 * shift < 0)
                    text[i] = dictionnary.at(indexOfChar - 2 * shift + dictionnary.length());

                else
                    text[i] = dictionnary.at(indexOfChar - 2 * shift);
            }
        }
    }


    return Protocol::Error::NoError;
}
