#pragma once
#include "Quote.h"

class SimpleQuote : public Quote
{
public:
    SimpleQuote(Real value = Null<Real>());
public:
    Real Value() const override;
    bool IsValid() const override;
    Real SetValue(Real value = Null<Real>());
    void Reset();
private:
    Real _value;
};

