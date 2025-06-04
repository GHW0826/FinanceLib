#pragma once
#include "pattern/Observable.h"
#include "quote/Quote.h"
#include "time/Date.h"
#include "handle.h"

// ��Ʈ��Ʈ������ ���� �⺻ ���� Ŭ����
// Bootstrapping: �ʱ�ȭ, Ư�� ������ ��� �Ǵ� ������ �ܰ������� ����� ��
// ������ �ý����̳� ��� ����(��: ������ Ŀ��, ���� �� ��)�� �ʱ�ȭ�ϱ� ����,
// ��� ������ ����� ������ �ϴ� ��� Ŭ����
/*! �� Ŭ������ ���ͷ� �(�Ǵ� �Ⱓ ����)�� ��Ʈ��Ʈ���ϴ� �� ���Ǵ� ������ǰ�鿡 ���� �߻�ȭ�� ����.
    ��Ʈ��Ʈ���ο� ���Ǵ� ���� Ŭ������,
    �ش� ������ǰ(��: ����, ���� ��)�� ���� �ν��Ͻ��� �����ϵ��� �����ϴ� ���� �ٶ���.
    �׷��� ��Ʈ��Ʈ���� �������� ���Ǵ� �˰����, 
    ���߿� ���� ��ǰ�� ���������� ���� ����� �� ����ϴ� �˰��� ���� �ϰ����� ������ �� �ִ�.
    �ٸ� ���� �����Ǵ� BootstrapHelper Ŭ�����鿡���� �� ��Ģ�� ������ ���������� �ʰ� �ִ�.
*/
template <class TS>
class BootstrapHelper : public Observer, public Observable
{

protected:
    Handle<Quote> _quote;
    TS* _termStructure;
    Date _earliestDate;
    Date _latestDate;
    Date _maturityDate;
    Date _latestRelevantDate;
    Date _pillarDate;
};

//! Bootstrap helper with date schedule relative to global evaluation date
/*! Derived classes must takes care of rebuilding the date schedule when
    the global evaluation date changes
*/
template <class TS>
class RelativeDateBootstrapHelper : public BootstrapHelper<TS> {
public:
    explicit RelativeDateBootstrapHelper(const std::variant<Spread, Handle<Quote>>& quote, bool updateDates = true);

    //! \name Observer interface
    //@{
    void update() override 
    {
        if (_updateDates && _evaluationDate != Settings::instance().evaluationDate()) {
            _evaluationDate = Settings::instance().evaluationDate();
            initializeDates();
        }
        BootstrapHelper<TS>::update();
    }
    //@}
protected:
    virtual void initializeDates() = 0;
    Date _evaluationDate;
    bool _updateDates;
};