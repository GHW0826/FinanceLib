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
    // ���� �帧 ���� �帧, ���� ��ȯ, ����, ���ͷ� ����, ����� ���� �� �پ��� ���� �ֱ��� (��, �б�, �ݱ� ��)
    Leg _cashflows;     
    // ���� ��ȯ(���� �Ǵ� ����)
    // �幰�� 1~2ȸ (�ַ� ����/����)
    // ���� ȸ��/���� ȸ�� �� ����ũ ���
    Leg _redemptions;   

    Date _maturityDate;
    Date _issueDate;
    mutable Real _settlementValue;
};

