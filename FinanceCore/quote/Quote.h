#pragma once
#include "pattern/Observable.h"
#include "utility/Null.h"

// 마켓 관찰을 위한 기본 가상 클래스.
class Quote : public Observable
{
public:
    virtual ~Quote() = default;
    // 현재 가치 반환
    virtual Real Value() const = 0;
    // Valid 여부
    virtual bool IsValid() const = 0;
};