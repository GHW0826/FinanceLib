#pragma once
<<<<<<< HEAD
#include "TimeUnit.h"
=======
#include "Frequency.h"
>>>>>>> 08ac318 (bond example process2)

class Period
{
public:
<<<<<<< HEAD
    Integer GetLength() const { return _length; }
    TimeUnit GetUnits() const { return _units; }
=======
    Period() = default;
    Period(Integer n, TimeUnit units)
        : _length(n), _units(units) 
    {}
    explicit Period(Frequency f);
    Integer length() const { return _length; }
    TimeUnit units() const { return _units; }
    Frequency frequency() const;
    Period& operator+=(const Period&);
    Period& operator-=(const Period&);
    Period& operator*=(Integer);
    Period& operator/=(Integer);
    void normalize();
    Period normalized() const;
>>>>>>> 08ac318 (bond example process2)
private:
    Integer _length = 0;
    TimeUnit _units = kDays;
};

