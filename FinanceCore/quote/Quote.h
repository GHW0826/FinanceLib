#pragma once
#include "pattern/Observable.h"
#include "utility/Null.h"

// 마켓 관찰을 위한 기본 가상 클래스.
// Quote는 금융 상품의 가격 정보 또는 시장 호가를 의미.
// 증권 실무에서의 의미는 맥락에 따라 다소 달라지지만, 공통적으로:
// 지금 시장에서 이 자산을 얼마에 사고 / 팔 수 있는지 나타내는 숫자
class Quote : public Observable
{
public:
    virtual ~Quote() = default;
    // 현재 가치 반환
    virtual Real Value() const = 0;
    // Valid 여부
    virtual bool IsValid() const = 0;
};