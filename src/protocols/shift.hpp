#ifndef SHIFT_HPP
#define SHIFT_HPP


#include "src/protocols/protocol.hpp"


class Shift : public Protocol
{
    public:
        explicit Shift();

        virtual Protocol::Error cipher(QString&) const override;
        virtual Protocol::Error decipher(QString&) const override;

    private:
        static const QString dictionnary;
};


#endif  // SHIFT_HPP
