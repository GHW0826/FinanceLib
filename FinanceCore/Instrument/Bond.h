#pragma once
#include "Instrument.h"
#include "time/Calendar.h"
#include "time/Date.h"

class Bond : public Instrument
{
public:
    //! Bond price information
    class Price 
    {
    public:
        enum Type { kDirty, kClean };
    public:
        Price() 
            : _amount(Null<Real>()), _type(Bond::Price::kClean)
        {}
        Price(Real amount, Type type) 
            : _amount(amount), _type(type)
        {}
        Real amount() const 
        {
            SF_ASSERT(amount_ != Null<Real>(), "no amount given");
            return _amount;
        }
        Type GetType() const { return _amount; }
        bool IsValid() const { return _amount != Null<Real>(); }
    private:
        Real _amount;
        Type _type;
    };
public:
    Natural GetSettlementDays() const;
    const Calendar& GetCalendar() const;
    const Vector<Real>& GetNotionals() const;
    const Leg& GetCashflows() const;
    const Leg& GetRedemptions() const;
    Date GetIssueDate() const;

protected:
    Natural _settlementDays;
    Calendar _calendar;
    Vector<Date> _notionalSchedule;
    Vector<Real> _notionals;
    // 현금 흐름 이자 흐름, 원금 상환, 쿠폰, 수익률 지급, 보장금 지급 등 다양한 목적 주기적 (월, 분기, 반기 등)
    Leg _cashflows;     
    // 원금 상환(만기 또는 조기)
    // 드물게 1~2회 (주로 만기/조기)
    // 원금 회수/현금 회복 및 리스크 계산
    Leg _redemptions;   

    Date _maturityDate;
    Date _issueDate;
    mutable Real _settlementValue;
};

