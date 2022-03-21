#ifndef GREEK_HPP
#define GREEK_HPP


#include "src/protocols/protocol.hpp"


class Greek : public Protocol
{
    public:
        explicit Greek();

        virtual Protocol::Error cipher(QString&) const override;
        virtual Protocol::Error decipher(QString&) const override;

    private:
        static const QString decryptedAlphabet;
        static const QString encryptedAlphabet;
};


#endif  // GREEK_HPP
