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
    bool IsEndOfMonth(const Date& d) const;
    Date EndOfMonth(const Date& d) const;
    bool IsHoliday(const Date& d) const;
    bool IsBusinessDay(const Date& d) const;
public:
    // 영업일이 아닌 날짜(휴일 또는 주말 등)를 지정한 규칙(BusinessDayConvention)에 따라 
    // 가까운 유효한 영업일로 보정해주는 함수.
    Date Adjust(const Date& date, BusinessDayConvention convention = kFollowing) const;

    // 주어진 날짜로부터 지정한 수의 영업일만큼 전진한 날짜를 반환합니다.
    // 원래 입력한 날짜는 변경되지 않습니다.
    Date Advance(const Date& date, Integer n, TimeUnit unit, BusinessDayConvention convention = kFollowing, bool endOfMonth = false) const;
    Date Advance(const Date& date, const Period& period, BusinessDayConvention convention = kFollowing, bool endOfMonth = false) const;
protected:
    SharedPtr<Impl> _impl;
};

