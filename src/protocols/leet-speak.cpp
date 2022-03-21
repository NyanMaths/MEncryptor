#include "src/protocols/leet-speak.hpp"


const QString LeetSpeak::decryptedAlphabet("abcdefghijklmnopqrstuvwxyz");
const QString LeetSpeak::encryptedAlphabet("48(?3ƒ6#]¿<1M^09QЯ57µVШ*φ2");

LeetSpeak::LeetSpeak () : Protocol()
{
    // Nothing to be done here
}


Protocol::Error LeetSpeak::cipher (QString& text) const
{
    text = text.toLower();

    for (qsizetype i(0) ; i != text.length() ; i++)
        for (qsizetype d(0) ; d != decryptedAlphabet.length() ; d++)
            if (decryptedAlphabet.at(d) == text.at(i))
                text[i] = encryptedAlphabet.at(d);

    return Protocol::Error::NoError;
}

Protocol::Error LeetSpeak::decipher (QString& text) const
{
    for (qsizetype i(0) ; i != text.length() ; i++)
    {
        if (text.at(i) == 'm')
            text[i] = 'M';
        if (text.at(i) == 'q')
            text[i] = 'Q';
        if (text.at(i) == 'v')
            text[i] = 'V';

        for (qsizetype d(0) ; d != encryptedAlphabet.length() ; d++)
            if (encryptedAlphabet.at(d) == text.at(i))
                text[i] = decryptedAlphabet.at(d);
    }

    return Protocol::Error::NoError;
}
