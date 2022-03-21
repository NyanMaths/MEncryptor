#ifndef MORSE_HPP
#define MORSE_HPP


#include "src/protocols/protocol.hpp"


class Morse : public Protocol
{
    public:
        explicit Morse();

        virtual Protocol::Error cipher(QString&) const override;
        virtual Protocol::Error decipher(QString&) const override;

    private:
        static const QString decryptedAlphabet;
        static const QVector<QString> encryptedAlphabet;
};


#endif  // MORSE_HPP
