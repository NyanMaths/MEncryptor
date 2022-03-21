#ifndef VIGENERE_HPP
#define VIGENERE_HPP


#include "src/protocols/protocol.hpp"


class Vigenere : public Protocol
{
    public:
        explicit Vigenere();

        virtual Protocol::Error cipher(QString&) const override;
        virtual Protocol::Error decipher(QString&) const override;

    private:
        static const QString dictionnary;
};


#endif  // VIGENERE_HPP
