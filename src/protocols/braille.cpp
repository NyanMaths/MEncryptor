#include "src/protocols/braille.hpp"


const QString Braille::decryptedAlphabet("0123456789abcdefghijklmnopqrstuvwxyzàâçèéêëîïôœùûü,;:.?!\"()'_/@*^");
const QVector<QString> Braille::encryptedAlphabet
{
    "⠠⠼",
    "⠠⠡",
    "⠠⠣",
    "⠠⠩",
    "⠠⠹",
    "⠠⠱",
    "⠠⠫",
    "⠠⠻",
    "⠠⠳",
    "⠠⠪",
    "⠁",
    "⠃",
    "⠉",
    "⠙",
    "⠑",
    "⠋",
    "⠛",
    "⠓",
    "⠊",
    "⠚",
    "⠅",
    "⠇",
    "⠍",
    "⠝",
    "⠕",
    "⠏",
    "⠟",
    "⠗",
    "⠎",
    "⠞",
    "⠥",
    "⠧",
    "⠺",
    "⠭",
    "⠽",
    "⠵",
    "⠷",
    "⠡",
    "⠯",
    "⠮",
    "⠿",
    "⠣",
    "⠫",
    "⠩",
    "⠻",
    "⠹",
    "⠪",
    "⠾",
    "⠱",
    "⠳",
    "⠂",
    "⠆",
    "⠒",
    "⠲",
    "⠢",
    "⠖",
    "⠶",
    "⠦",
    "⠴",
    "⠄",
    "⠤",
    "⠌",
    "⠜",
    "⠔",
    "⠈"
};

Braille::Braille () : Protocol()
{
    // Nothing to be done here
}


Protocol::Error Braille::cipher (QString& text) const
{
    text = text.toLower();
    QString encryptedText;

    for (qsizetype i(0) ; i != text.length() ; i++)
    {
        if (decryptedAlphabet.contains(text.at(i)))
            encryptedText += encryptedAlphabet.at(decryptedAlphabet.indexOf(text.at(i)));

        else
            encryptedText += text.at(i);
    }

    text = encryptedText;

    return Protocol::Error::NoError;
}

Protocol::Error Braille::decipher (QString& text) const
{
    for (qsizetype i(0) ; i != text.length() ; i++)
        for (qsizetype d(0) ; d != encryptedAlphabet.length() ; d++)
            if (encryptedAlphabet.at(d) == text.at(i))
                text[i] = decryptedAlphabet.at(d);

    return Protocol::Error::NoError;
}
