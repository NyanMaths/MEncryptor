#include "src/protocols/reversed-words.hpp"


ReversedWords::ReversedWords () : Protocol()
{
    // Nothing to be done here
}


Protocol::Error ReversedWords::cipher (QString& text) const
{
    QVector<QString> lines(text.split('\n'));
    QVector<QString> words;

    for (qsizetype i(0) ; i != lines.length() ; i++)
    {
        words = lines.at(i).split(' ');

        for (qsizetype wordIndex(0) ; wordIndex != words.length() ; wordIndex++)
            words[wordIndex] = reversed(words.at(wordIndex));

        lines[i] = words.join(' ');
    }

    text = lines.join('\n');

    return Protocol::Error::NoError;
}

Protocol::Error ReversedWords::decipher (QString& text) const
{
    QVector<QString> lines(text.split('\n'));
    QVector<QString> words;

    for (qsizetype i(0) ; i != lines.length() ; i++)
    {
        words = lines[i].split(' ');

        for (qsizetype wordIndex(0) ; wordIndex != words.length() ; wordIndex++)
            words[wordIndex] = reversed(words.at(wordIndex));

        lines[i] = words.join(' ');
    }

    text = lines.join('\n');

    return Protocol::Error::NoError;
}
