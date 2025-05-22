#include "pch.h"
#include "Date.h"

#include <iomanip>

Date::Date()
	: _serialNumber(Date::serial_type(0))
{}

Date::Date(Day d, Month m, Year y) 
{
    SF_ASSERT(y > 1900 && y < 2200, "year " << y << " out of bound. It must be in [1901,2199]");
    SF_ASSERT(Integer(m) > 0 && Integer(m) < 13, "month " << Integer(m) << " outside January-December range [1,12]");

    bool leap = IsLeap(y);
    Day len = MonthLength(m, leap), offset = MonthOffset(m, leap);
    SF_ASSERT(d <= len && d > 0, "day outside month (" << Integer(m) << ") day-range " << "[1," << len << "]");

    _serialNumber = d + offset + YearOffset(y);
}

Date::Date(Date::serial_type serialNumber)
    : _serialNumber(serialNumber)
{
    CheckSerialNumber(serialNumber);
}

Date& Date::operator+=(Date::serial_type days)
{
    Date::serial_type serial = _serialNumber + days;
    CheckSerialNumber(serial);
    _serialNumber = serial;
    return *this;
}

Date& Date::operator+=(const Period& period)
{
    _serialNumber = Advance(*this, period.GetLength(), period.GetUnits()).SerialNumber();
    return *this;
}

Date& Date::operator-=(Date::serial_type days)
{
    Date::serial_type serial = _serialNumber - days;
    CheckSerialNumber(serial);
    _serialNumber = serial;
    return *this;
}

Date& Date::operator-=(const Period& period)
{
    _serialNumber = Advance(*this, -period.GetLength(), period.GetUnits()).SerialNumber();
    return *this;
}

Date& Date::operator++()
{
    Date::serial_type serial = _serialNumber + 1;
    CheckSerialNumber(serial);
    _serialNumber = serial;
    return *this;
}

Date Date::operator++(int)
{
    Date old(*this);
    ++*this;
    return old;
}

Date& Date::operator--()
{
    Date::serial_type serial = _serialNumber - 1;
    CheckSerialNumber(serial);
    _serialNumber = serial;
    return *this;
}

Date Date::operator--(int)
{
    Date old(*this);
    --*this;
    return old;
}

Date Date::operator+(Date::serial_type days) const
{
    return Date(_serialNumber + days);
}

Date Date::operator+(const Period& period) const
{
    return Advance(*this, period.GetLength(), period.GetUnits());
}

Date Date::operator-(Date::serial_type days) const
{
    return Date(_serialNumber - days);
}

Date Date::operator-(const Period& period) const
{
    return Advance(*this, -period.GetLength(), period.GetUnits());
}

Day Date::DayOfYear() const
{
    return Day();
}

Month Date::GetMonth() const
{
    Day d = DayOfYear(); // dayOfYear is 1 based
    Integer m = d / 30 + 1;
    bool leap = IsLeap(GetYear());
    while (d <= MonthOffset(Month(m), leap))
        --m;
    while (d > MonthOffset(Month(m + 1), leap)) // NOLINT(misc-misplaced-widening-cast)
        ++m;
    return Month(m);
}

Year Date::GetYear() const
{
    Year y = (_serialNumber / 365) + 1900;
    // yearOffset(y) is December 31st of the preceding year
    if (_serialNumber <= YearOffset(y))
        --y;
    return y;
}

Date::serial_type Date::SerialNumber() const
{
    return Date::serial_type();
}

Date Date::Advance(const Date& date, Integer n, TimeUnit units)  const
{
    switch (units) {
    case kDays:
        return date + n;
    case kWeeks:
        return date + 7 * n;
    case kMonths: {
        Day d = date.DayOfMonth();
        Integer m = Integer(date.GetMonth()) + n;
        Year y = date.GetYear();
        while (m > 12) {
            m -= 12;
            y += 1;
        }
        while (m < 1) {
            m += 12;
            y -= 1;
        }

        SF_ASSERT(y >= 1900 && y <= 2199, "year " << y << " out of bounds. " << "It must be in [1901,2199]");

        Integer length = MonthLength(Month(m), IsLeap(y));
        if (d > length)
            d = length;
        return { d, Month(m), y };
    }
    case kYears: {
        Day d = date.DayOfMonth();
        Month m = date.GetMonth();
        Year y = date.GetYear() + n;

        SF_ASSERT(y >= 1900 && y <= 2199, "year " << y << " out of bounds. " << "It must be in [1901,2199]");
        if (d == 29 && m == kFebruary && !IsLeap(y))
            d = 28;
        return { d, m, y };
    }
    default:
        SF_FAIL("undefined time units");
    }

}
bool Date::IsLeap(Year y)
{
    static const bool YearIsLeap[] = {
        // 1900-1909
        true,false,false,false, true,false,false,false, true,false,
        // 1910-1919
        false,false, true,false,false,false, true,false,false,false,
        // 1920-1929
        true,false,false,false, true,false,false,false, true,false,
        // 1930-1939
        false,false, true,false,false,false, true,false,false,false,
        // 1940-1949
        true,false,false,false, true,false,false,false, true,false,
        // 1950-1959
        false,false, true,false,false,false, true,false,false,false,
        // 1960-1969
        true,false,false,false, true,false,false,false, true,false,
        // 1970-1979
        false,false, true,false,false,false, true,false,false,false,
        // 1980-1989
        true,false,false,false, true,false,false,false, true,false,
        // 1990-1999
        false,false, true,false,false,false, true,false,false,false,
        // 2000-2009
        true,false,false,false, true,false,false,false, true,false,
        // 2010-2019
        false,false, true,false,false,false, true,false,false,false,
        // 2020-2029
        true,false,false,false, true,false,false,false, true,false,
        // 2030-2039
        false,false, true,false,false,false, true,false,false,false,
        // 2040-2049
        true,false,false,false, true,false,false,false, true,false,
        // 2050-2059
        false,false, true,false,false,false, true,false,false,false,
        // 2060-2069
        true,false,false,false, true,false,false,false, true,false,
        // 2070-2079
        false,false, true,false,false,false, true,false,false,false,
        // 2080-2089
        true,false,false,false, true,false,false,false, true,false,
        // 2090-2099
        false,false, true,false,false,false, true,false,false,false,
        // 2100-2109
        false,false,false,false, true,false,false,false, true,false,
        // 2110-2119
        false,false, true,false,false,false, true,false,false,false,
        // 2120-2129
        true,false,false,false, true,false,false,false, true,false,
        // 2130-2139
        false,false, true,false,false,false, true,false,false,false,
        // 2140-2149
        true,false,false,false, true,false,false,false, true,false,
        // 2150-2159
        false,false, true,false,false,false, true,false,false,false,
        // 2160-2169
        true,false,false,false, true,false,false,false, true,false,
        // 2170-2179
        false,false, true,false,false,false, true,false,false,false,
        // 2180-2189
        true,false,false,false, true,false,false,false, true,false,
        // 2190-2199
        false,false, true,false,false,false, true,false,false,false,
        // 2200
        false
    };
    SF_ASSERT(y >= 1900 && y <= 2200, "year outside valid range");
    return YearIsLeap[y - 1900];
}

Date Date::MinDate() {
    static const Date minimumDate(MinimumSerialNumber());
    return minimumDate;
}

Date Date::MaxDate() {
    static const Date maximumDate(MaximumSerialNumber());
    return maximumDate;
}

<<<<<<< HEAD
Date Date::StartOfMonth(const Date& d)
{
    return Date();
}

bool Date::IsStartOfMonth(const Date& d)
{
    return false;
}

Date Date::EndOfMonth(const Date& d)
{
    return Date();
=======
Date Date::EndOfMonth(const Date& d)
{
    Month m = d.GetMonth();
    Year y = d.GetYear();
    return { MonthLength(m, IsLeap(y)), m, y };
>>>>>>> 08ac318 (bond example process2)
}

bool Date::IsEndOfMonth(const Date& d)
{
<<<<<<< HEAD
    return false;
}

Date Date::NextWeekday(const Date& d, Weekday w)
{
    return Date();
=======
    return (d.DayOfMonth() == MonthLength(d.GetMonth(), IsLeap(d.GetYear())));
>>>>>>> 08ac318 (bond example process2)
}

void Date::CheckSerialNumber(Date::serial_type serialNumber) {
    SF_ASSERT(serialNumber >= MinimumSerialNumber() && serialNumber <= MaximumSerialNumber(),
        "Date's serial number (" << serialNumber << ") outside " <<
        "allowed range [" << MinimumSerialNumber() << "-" << MaximumSerialNumber() << "],"
        << "i.e. [" << MinDate() << "-" << MaxDate() << "]");
}

Date::serial_type Date::MinimumSerialNumber() {
    // 1901/01/01
    return 367;      
}

Date::serial_type Date::MaximumSerialNumber() {
    // 2199/12/31
    return 109574;
}

Integer Date::MonthLength(Month m, bool leapYear)
{
    // 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12¿ù
    static const Integer MonthLength[] = {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };
    static const Integer MonthLeapLength[] = {
        31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };
    return (leapYear ? MonthLeapLength[m - 1] : MonthLength[m - 1]);
}

Integer Date::MonthOffset(Month m, bool leapYear)
{
    static const Integer MonthOffset[] = {
          0,  31,  59,  90, 120, 151,   // 1 - 6
        181, 212, 243, 273, 304, 334,   // 7 - 12
        365     // used in dayOfMonth to bracket day
    };
    static const Integer MonthLeapOffset[] = {
          0,  31,  60,  91, 121, 152,   // 1 - 6
        182, 213, 244, 274, 305, 335,   // 7 - 12
        366     // used in dayOfMonth to bracket day
    };
    return (leapYear ? MonthLeapOffset[m - 1] : MonthOffset[m - 1]);
}

Date::serial_type Date::YearOffset(Year y)
{
    static const Date::serial_type YearOffset[] = {
        // 1900-1909
        0,  366,  731, 1096, 1461, 1827, 2192, 2557, 2922, 3288,
        // 1910-1919
        3653, 4018, 4383, 4749, 5114, 5479, 5844, 6210, 6575, 6940,
        // 1920-1929
        7305, 7671, 8036, 8401, 8766, 9132, 9497, 9862,10227,10593,
        // 1930-1939
        10958,11323,11688,12054,12419,12784,13149,13515,13880,14245,
        // 1940-1949
        14610,14976,15341,15706,16071,16437,16802,17167,17532,17898,
        // 1950-1959
        18263,18628,18993,19359,19724,20089,20454,20820,21185,21550,
        // 1960-1969
        21915,22281,22646,23011,23376,23742,24107,24472,24837,25203,
        // 1970-1979
        25568,25933,26298,26664,27029,27394,27759,28125,28490,28855,
        // 1980-1989
        29220,29586,29951,30316,30681,31047,31412,31777,32142,32508,
        // 1990-1999
        32873,33238,33603,33969,34334,34699,35064,35430,35795,36160,
        // 2000-2009
        36525,36891,37256,37621,37986,38352,38717,39082,39447,39813,
        // 2010-2019
        40178,40543,40908,41274,41639,42004,42369,42735,43100,43465,
        // 2020-2029
        43830,44196,44561,44926,45291,45657,46022,46387,46752,47118,
        // 2030-2039
        47483,47848,48213,48579,48944,49309,49674,50040,50405,50770,
        // 2040-2049
        51135,51501,51866,52231,52596,52962,53327,53692,54057,54423,
        // 2050-2059
        54788,55153,55518,55884,56249,56614,56979,57345,57710,58075,
        // 2060-2069
        58440,58806,59171,59536,59901,60267,60632,60997,61362,61728,
        // 2070-2079
        62093,62458,62823,63189,63554,63919,64284,64650,65015,65380,
        // 2080-2089
        65745,66111,66476,66841,67206,67572,67937,68302,68667,69033,
        // 2090-2099
        69398,69763,70128,70494,70859,71224,71589,71955,72320,72685,
        // 2100-2109
        73050,73415,73780,74145,74510,74876,75241,75606,75971,76337,
        // 2110-2119
        76702,77067,77432,77798,78163,78528,78893,79259,79624,79989,
        // 2120-2129
        80354,80720,81085,81450,81815,82181,82546,82911,83276,83642,
        // 2130-2139
        84007,84372,84737,85103,85468,85833,86198,86564,86929,87294,
        // 2140-2149
        87659,88025,88390,88755,89120,89486,89851,90216,90581,90947,
        // 2150-2159
        91312,91677,92042,92408,92773,93138,93503,93869,94234,94599,
        // 2160-2169
        94964,95330,95695,96060,96425,96791,97156,97521,97886,98252,
        // 2170-2179
        98617,98982,99347,99713,100078,100443,100808,101174,101539,101904,
        // 2180-2189
        102269,102635,103000,103365,103730,104096,104461,104826,105191,105557,
        // 2190-2199
        105922,106287,106652,107018,107383,107748,108113,108479,108844,109209,
        // 2200
        109574
    };
    return YearOffset[y - 1900];
}

Weekday Date::GetWeekday() const
{
    return Weekday();
}

Day Date::DayOfMonth() const
{
    return Day();
}

#pragma region OP

inline bool operator==(const Date& d1, const Date& d2) {
    return (d1.SerialNumber() == d2.SerialNumber());
}

inline bool operator!=(const Date& d1, const Date& d2) {
    return (d1.SerialNumber() != d2.SerialNumber());
}

inline bool operator<(const Date& d1, const Date& d2) {
    return (d1.SerialNumber() < d2.SerialNumber());
}

inline bool operator<=(const Date& d1, const Date& d2) {
    return (d1.SerialNumber() <= d2.SerialNumber());
}

inline bool operator>(const Date& d1, const Date& d2) {
    return (d1.SerialNumber() > d2.SerialNumber());
}

inline bool operator>=(const Date& d1, const Date& d2) {
    return (d1.SerialNumber() >= d2.SerialNumber());
}


std::ostream& operator<<(std::ostream& out, const Date& d) {
    return out << LongDate(d);
}

#pragma endregion



#pragma region IO

struct FormatResetter { 
    // NOLINT(cppcoreguidelines-special-member-functions)
    // An instance of this object will have undefined behaviour
    // if the object out passed in the constructor is destroyed
    // before this instance
    struct nopunct : std::numpunct<char> {
        std::string do_grouping() const override { return ""; }
    };
    explicit FormatResetter(std::ostream& out)
        : out_(&out), flags_(out.flags()), filler_(out.fill()),
        loc_(out.getloc())
    {
        std::locale loc(out.getloc(), new nopunct);
        out.imbue(loc);
        out << std::resetiosflags(
            std::ios_base::adjustfield | std::ios_base::basefield |
            std::ios_base::floatfield | std::ios_base::showbase |
            std::ios_base::showpos | std::ios_base::uppercase);
        out << std::right;
    }
    ~FormatResetter() {
        out_->flags(flags_);
        out_->fill(filler_);
        out_->imbue(loc_);
    }
    std::ostream* out_;
    std::ios_base::fmtflags flags_;
    char filler_;
    std::locale loc_;
};


// ShortDateHolder
std::ostream& operator<<(std::ostream& out, const ShortDateHolder& holder) 
{
    const Date& d = holder.d;
    if (d == Date()) {
        out << "null date";
    }
    else {
        FormatResetter resetter(out);
        Integer dd = d.DayOfMonth();
        Integer mm = Integer(d.GetMonth());
        Integer yyyy = d.GetYear();
        char filler = out.fill();
        out << std::setw(2) << std::setfill('0') << mm << "/";
        out << std::setw(2) << std::setfill('0') << dd << "/";
        out << yyyy;
        out.fill(filler);
    }
    return out;
}

// LongDateHolder
std::ostream& operator<<(std::ostream& out, const LongDateHolder& holder) 
{
    const Date& d = holder.d;
    if (d == Date()) {
        out << "null date";
    }
    else {
        FormatResetter resetter(out);
        out << d.GetMonth() << " ";
        out << ordinal(d.DayOfMonth()) << ", ";
        out << d.GetYear();
    }
    return out;
}

// IsoDateHolder
std::ostream& operator<<(std::ostream& out, const IsoDateHolder& holder) 
{
    const Date& d = holder.d;
    if (d == Date()) {
        out << "null date";
    }
    else {
        FormatResetter resetter(out);
        Integer dd = d.DayOfMonth();
        Integer mm = Integer(d.GetMonth());
        Integer yyyy = d.GetYear();
        out << yyyy << "-";
        out << std::setw(2) << std::setfill('0') << mm << "-";
        out << std::setw(2) << std::setfill('0') << dd;
    }
    return out;
}

// OrdinalHolder
std::ostream& operator<<(std::ostream& out, const OrdinalHolder& holder) {
    Size n = holder.n;
    out << n;
    if (n == Size(11) || n == Size(12) || n == Size(13)) {
        out << "th";
    }
    else {
        switch (n % 10) {
        case 1:  out << "st";  break;
        case 2:  out << "nd";  break;
        case 3:  out << "rd";  break;
        default: out << "th";
        }
    }
    return out;
}

ShortDateHolder ShortDate(const Date& d) {
    return ShortDateHolder(d);
}

LongDateHolder LongDate(const Date& d) {
    return LongDateHolder(d);
}

IsoDateHolder IsoDate(const Date& d) {
    return IsoDateHolder(d);
}

#pragma endregion
