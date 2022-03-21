#ifndef VALUES_HPP
#define VALUES_HPP


#include "src/protocols/protocol.hpp"


class Values : public Protocol
{
    public:
        explicit Values();

        virtual Protocol::Error cipher(QString&) const override;
        virtual Protocol::Error decipher(QString&) const override;

    private:
        static const QString decryptedAlphabet;
        static const QVector<QString> encryptedAlphabet;
};


#endif  // VALUES_HPP
