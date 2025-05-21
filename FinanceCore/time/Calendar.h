#pragma once
#include "Date.h"
#include "TimeUnit.h"
#include "BusinessDayConvention.h"
#include "Period.h"

class Calendar
{
protected:
    class Impl 
    {
    public:
        virtual ~Impl() = default;
        virtual std::string GetName() const = 0;
        virtual bool IsBusinessDay(const Date&) const = 0;
        virtual bool IsWeekend(Weekday) const = 0;
    public:
        std::set<Date> _addedHolidays;
        std::set<Date> _removedHolidays;
    };
    class WesternImpl : public Impl 
    {
    public:
        bool IsWeekend(Weekday) const override;
        static Day EasterMonday(Year);
    };
    class OrthodoxImpl : public Impl 
    {
    public:
        bool IsWeekend(Weekday) const override;
        static Day EasterMonday(Year);
    };
public:
    Date Advance(const Date&, Integer n, TimeUnit unit, BusinessDayConvention convention = kFollowing, bool endOfMonth = false) const;
    Date Advance(const Date& date, const Period& period, BusinessDayConvention convention = kFollowing, bool endOfMonth = false) const;
protected:
    SharedPtr<Impl> _impl;
};

