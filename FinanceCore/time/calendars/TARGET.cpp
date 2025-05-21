#include "pch.h"
#include "TARGET.h"

TARGET::TARGET()
{
	static SharedPtr<Calendar::Impl> impl = MakeShared<TARGET::Impl>();
	_impl = impl;
}

bool TARGET::Impl::IsBusinessDay(const Date& date) const
{
    Weekday w = date.Weekday();
    Day d = date.DayOfMonth();
    Day dd = date.DayOfYear();
    Month m = date.GetMonth();
    Year y = date.GetYear();
    Day em = EasterMonday(y);
    if (IsWeekend(w)
        // New Year's Day
        || (d == 1 && m == Month::kJanuary)
        // Good Friday
        || (dd == em - 3 && y >= 2000)
        // Easter Monday
        || (dd == em && y >= 2000)
        // Labour Day
        || (d == 1 && m == Month::kMay && y >= 2000)
        // Christmas
        || (d == 25 && m == Month::kDecember)
        // Day of Goodwill
        || (d == 26 && m == Month::kDecember && y >= 2000)
        // December 31st, 1998, 1999, and 2001 only
        || (d == 31 && m == Month::kDecember && (y == 1998 || y == 1999 || y == 2001)))
        return false;
    return true;
}
