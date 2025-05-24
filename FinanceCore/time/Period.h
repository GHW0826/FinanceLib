#pragma once
#include "TimeUnit.h"

class Period
{
public:
    Integer GetLength() const { return _length; }
    TimeUnit GetUnits() const { return _units; }
private:
    Integer _length = 0;
    TimeUnit _units = kDays;
};

