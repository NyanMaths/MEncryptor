#ifndef UNICODE_VALUES_HPP
#define UNICODE_VALUES_HPP


#include "src/protocols/protocol.hpp"


class UnicodeValues : public Protocol
{
    public:
        explicit UnicodeValues();

        virtual Protocol::Error cipher(QString&) const override;
        virtual Protocol::Error decipher(QString&) const override;
};


#endif  // UNICODE_VALUES_HPP
