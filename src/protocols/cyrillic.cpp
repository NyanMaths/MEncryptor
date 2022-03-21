#include "src/protocols/cyrillic.hpp"


const QString Cyrillic::decryptedAlphabet("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
const QString Cyrillic::encryptedAlphabet("абцдефгхийклмнопярстуввхызАБЦДЕФГХИЙКЛМНОПЯРСТУВВХЫЗ");

Cyrillic::Cyrillic () : Protocol()
{
    // Nothing to be done here
}


Protocol::Error Cyrillic::cipher (QString& text) const
{
    for (qsizetype i(0) ; i != text.length() ; i++)
        for (qsizetype d(0) ; d != decryptedAlphabet.length() ; d++)
            if (decryptedAlphabet.at(d) == text.at(i))
                text[i] = encryptedAlphabet.at(d);

    return Protocol::Error::NoError;
}

Protocol::Error Cyrillic::decipher (QString& text) const
{
    for (qsizetype i(0) ; i != text.length() ; i++)
    {
        for (qsizetype d(0) ; d != encryptedAlphabet.length() ; d++)
            if (encryptedAlphabet.at(d) == text.at(i))
                text[i] = decryptedAlphabet.at(d);
    }

    return Protocol::Error::NoError;
}
