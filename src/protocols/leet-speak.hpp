#ifndef LEETSPEAK_HPP
#define LEETSPEAK_HPP


#include "src/protocols/protocol.hpp"


class LeetSpeak : public Protocol
{
    public:
        explicit LeetSpeak();

        virtual Protocol::Error cipher(QString&) const override;
        virtual Protocol::Error decipher(QString&) const override;

    private:
        static const QString decryptedAlphabet;
        static const QString encryptedAlphabet;
};


#endif  // LEETSPEAK_HPP
