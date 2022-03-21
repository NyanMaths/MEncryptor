#ifndef CYRILLIC_HPP
#define CYRILLIC_HPP


#include "src/protocols/protocol.hpp"


class Cyrillic : public Protocol
{
    public:
        explicit Cyrillic();

        virtual Protocol::Error cipher(QString&) const override;
        virtual Protocol::Error decipher(QString&) const override;

    private:
        static const QString decryptedAlphabet;
        static const QString encryptedAlphabet;
};


#endif  // CYRILLIC_HPP
