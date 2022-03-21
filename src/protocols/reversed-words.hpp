#ifndef REVERSED_WORDS_HPP
#define REVERSED_WORDS_HPP


#include "src/protocols/protocol.hpp"


class ReversedWords : public Protocol
{
    public:
        explicit ReversedWords();

        virtual Protocol::Error cipher(QString&) const override;
        virtual Protocol::Error decipher(QString&) const override;
};


#endif  // REVERSED_WORDS_HPP
