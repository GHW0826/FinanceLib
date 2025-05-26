#pragma once
#include "pattern/Observable.h"
#include "utility/Null.h"

// ���� ������ ���� �⺻ ���� Ŭ����.
class Quote : public Observable
{
public:
    virtual ~Quote() = default;
    // ���� ��ġ ��ȯ
    virtual Real Value() const = 0;
    // Valid ����
    virtual bool IsValid() const = 0;
};