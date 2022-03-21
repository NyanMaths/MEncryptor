#ifndef BRAILLE_HPP
#define BRAILLE_HPP


#include "src/protocols/protocol.hpp"


class Braille : public Protocol
{
    public:
        explicit Braille();

        virtual Protocol::Error cipher(QString&) const override;
        virtual Protocol::Error decipher(QString&) const override;

    private:
        static const QString decryptedAlphabet;
        static const QVector<QString> encryptedAlphabet;
};


#endif  // BRAILLE_HPP
