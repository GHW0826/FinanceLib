#include "pch.h"
#include "SimpleQuote.h"

SimpleQuote::SimpleQuote(Real value)
    : _value(value)
{
}

Real SimpleQuote::Value() const
{
    SF_ASSERT(IsValid(), "invalid SimpleQuote");
    return _value;
}

bool SimpleQuote::IsValid() const
{
    return _value != Null<Real>();
}

Real SimpleQuote::SetValue(Real value)
{
    Real diff = value - _value;
    if (diff != 0.0) {
        _value = value;
        NotifyObservers();
    }
    return diff;
}

void SimpleQuote::Reset()
{
    SetValue(Null<Real>());
}
