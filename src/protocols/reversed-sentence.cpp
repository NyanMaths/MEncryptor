#include "src/protocols/reversed-sentence.hpp"


ReversedSentence::ReversedSentence () : Protocol()
{
    // Nothing to be done here
}


Protocol::Error ReversedSentence::cipher (QString& text) const
{
    text = Protocol::reversed(text);

    return Protocol::Error::NoError;
}

Protocol::Error ReversedSentence::decipher (QString& text) const
{
    text = Protocol::reversed(text);

    return Protocol::Error::NoError;
}
