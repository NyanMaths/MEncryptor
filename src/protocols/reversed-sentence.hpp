#ifndef REVERSED_SENTENCE_HPP
#define REVERSED_SENTENCE_HPP


#include "src/protocols/protocol.hpp"


class ReversedSentence : public Protocol
{
    public:
        explicit ReversedSentence();

        virtual Protocol::Error cipher(QString&) const override;
        virtual Protocol::Error decipher(QString&) const override;
};


#endif  // REVERSED_SENTENCE_HPP
