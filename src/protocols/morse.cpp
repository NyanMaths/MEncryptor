#include <QRegularExpression>

#include "src/protocols/morse.hpp"


const QString Morse::decryptedAlphabet("abcdeéfghijklmnopqrstuvwxyz1234567890.,:?'-/()\"=+×@");
const QVector<QString> Morse::encryptedAlphabet
{
    ".-",
    "-...",
    "-.-.",
    "-..",
    ".",
    "..-..",
    "..-.",
    "--.",
    "....",
    "..",
    ".---",
    "-.-",
    ".-..",
    "--",
    "-.",
    "---",
    ".--.",
    "--.-",
    ".-.",
    "...",
    "-",
    "..-",
    "...-",
    ".--",
    "-..-",
    "-.--",
    "--..",
    ".----",
    "..---",
    "...--",
    "....-",
    ".....",
    "-....",
    "--...",
    "---..",
    "----.",
    "-----",
    ".-.-.-",
    "--..--",
    "---...",
    "..--..",
    ".----.",
    "-....-",
    "-..-.",
    "-.--.",
    "-.--.-",
    ".-..-.",
    "-...-",
    ".-.-.",
    "-..-",
    ".--.-."
};

Morse::Morse () : Protocol()
{
    // Nothing to be done here
}

#include <QMessageBox>
Protocol::Error Morse::cipher (QString& text) const
{
    QVector<QString> encryptedMessage(text.toLower().replace('\n', ' ').replace(QRegularExpression(" +"), " ").split(""));
    encryptedMessage.removeAt(0);
    encryptedMessage.removeAt(encryptedMessage.length() - 1);


    qsizetype indexOfChar;
    for (qsizetype i(0) ; i != encryptedMessage.length() ; i++)
    {
        indexOfChar = decryptedAlphabet.indexOf(encryptedMessage.at(i));

        if (indexOfChar != -1)
            encryptedMessage[i] = encryptedAlphabet.at(indexOfChar);
    }

    text = encryptedMessage.replaceInStrings(QRegularExpression(" "), "/").join(" ");

    return Protocol::Error::NoError;
}

Protocol::Error Morse::decipher (QString& text) const
{
    QVector<QString> words(text.split('/'));
    QVector<QString> word;

    qsizetype indexOfChar;
    for (qsizetype wordIndex(0) ; wordIndex != words.length() ; wordIndex++)
    {
        word = words.at(wordIndex).split(' ');

        for (qsizetype i(0) ; i != word.length() ; i++)
        {
            indexOfChar = encryptedAlphabet.indexOf(word.at(i));

            if (indexOfChar != -1)
                word[i] = decryptedAlphabet.at(indexOfChar);
        }

        words[wordIndex] = word.join("");
    }

    text = words.join(' ');

    return Protocol::Error::NoError;
}
