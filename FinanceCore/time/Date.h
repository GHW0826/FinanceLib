#pragma once
#include <cstdint>

#include "Period.h"
#include "Weekday.h"
#include "TimeUnit.h"

enum Month {
    kJanuary = 1,
    kFebruary = 2,
    kMarch = 3,
    kApril = 4,
    kMay = 5,
    kJune = 6,
    kJuly = 7,
    kAugust = 8,
    kSeptember = 9,
    kOctober = 10,
    kNovember = 11,
    kDecember = 12,
    kJan = 1,
    kFeb = 2,
    kMar = 3,
    kApr = 4,
    kJun = 6,
    kJul = 7,
    kAug = 8,
    kSep = 9,
    kOct = 10,
    kNov = 11,
    kDec = 12
};

using Day = Integer;
using Year = Integer;

class Date
{
    using serial_type = std::int_fast32_t;
public:
    Date();
    Date(Day d, Month m, Year y);
    explicit Date(Date::serial_type serialNumber);
public:
    Date& operator+=(Date::serial_type days);
    Date& operator+=(const Period&);
    Date& operator-=(Date::serial_type days);
    Date& operator-=(const Period&);
    Date& operator++();
    Date operator++(int);
    Date& operator--();
    Date operator--(int);
    Date operator+(Date::serial_type days) const;
    Date operator+(const Period&) const;
    Date operator-(Date::serial_type days) const;
    Date operator-(const Period&) const;
public:
    Date Advance(const Date& date, Integer n, TimeUnit units) const;
    static bool IsLeap(Year y);
    // 현재 관리하는 최소 날짜 반환 (1901/01/01)
    static Date MinDate();
    // 현재 관리하는 최대 날짜 반환 (2199/12/31)
    static Date MaxDate();
    static Date StartOfMonth(const Date& d);
    static bool IsStartOfMonth(const Date& d);
    static Date EndOfMonth(const Date& d);
    static bool IsEndOfMonth(const Date& d);
    static Date NextWeekday(const Date& d, Weekday w);
private:
    static void CheckSerialNumber(Date::serial_type serialNumber);
    static Date::serial_type MinimumSerialNumber();
    static Date::serial_type MaximumSerialNumber();

    static Integer MonthLength(Month m, bool leapYear);
    static Integer MonthOffset(Month m, bool leapYear);
    static Date::serial_type YearOffset(Year y);
public:
    Weekday GetWeekday() const;
    Day DayOfMonth() const;
    // One-based (Jan 1st = 1)
    Day DayOfYear() const;
    Month GetMonth() const;
    Year GetYear() const;
    Date::serial_type SerialNumber() const;
private:
    Date::serial_type _serialNumber;
};


#pragma region OP

bool operator==(const Date&, const Date&);
bool operator!=(const Date&, const Date&);
bool operator<(const Date&, const Date&);
bool operator<=(const Date&, const Date&);
bool operator>(const Date&, const Date&);
bool operator>=(const Date&, const Date&);

std::ostream& operator<<(std::ostream&, const Date&);

#pragma endregion

#pragma region IO


struct ShortDateHolder {
    explicit ShortDateHolder(const Date d) : d(d) {}
    Date d;
};
std::ostream& operator<<(std::ostream&, const ShortDateHolder&);

struct LongDateHolder {
    explicit LongDateHolder(const Date& d) : d(d) {}
    Date d;
};
std::ostream& operator<<(std::ostream&, const LongDateHolder&);

struct IsoDateHolder {
    explicit IsoDateHolder(const Date& d) : d(d) {}
    Date d;
};
std::ostream& operator<<(std::ostream&, const IsoDateHolder&);

struct OrdinalHolder {
    explicit OrdinalHolder(Size n) : n(n) {}
    Size n;
};
std::ostream& operator<<(std::ostream&, const OrdinalHolder&);

inline OrdinalHolder ordinal(Size n) {
    return OrdinalHolder(n);
}

//format (mm/dd/yyyy)
ShortDateHolder ShortDate(const Date&);

//format (Month ddth, yyyy)
LongDateHolder LongDate(const Date&);

//format (yyyy-mm-dd)
IsoDateHolder IsoDate(const Date&);

#pragma endregion
