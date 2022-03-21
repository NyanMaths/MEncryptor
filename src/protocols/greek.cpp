#include "src/protocols/greek.hpp"


const QString Greek::decryptedAlphabet("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
const QString Greek::encryptedAlphabet("αβςδεƒgнιjκℓмηορφяsτμvωχλζΑΒΣΔΕƑGНΙJΚℓМΗΟΡΦЯSΤΜVΩΧΛΖ");

Greek::Greek () : Protocol()
{
    // Nothing to be done here
}


Protocol::Error Greek::cipher (QString& text) const
{
    for (qsizetype i(0) ; i != text.length() ; i++)
        for (qsizetype d(0) ; d != decryptedAlphabet.length() ; d++)
            if (decryptedAlphabet.at(d) == text.at(i))
                text[i] = encryptedAlphabet.at(d);

    return Protocol::Error::NoError;
}

Protocol::Error Greek::decipher (QString& text) const
{
    for (qsizetype i(0) ; i != text.length() ; i++)
        for (qsizetype d(0) ; d != encryptedAlphabet.length() ; d++)
            if (encryptedAlphabet.at(d) == text.at(i))
                text[i] = decryptedAlphabet.at(d);

    return Protocol::Error::NoError;
}
