#pragma once
#include "pattern/Observable.h"
#include "quote/Quote.h"
#include "time/Date.h"
#include "handle.h"

// 부트스트래핑을 위한 기본 헬퍼 클래스
// Bootstrapping: 초기화, 특히 복잡한 계산 또는 구조를 단계적으로 세우는 것
// 복잡한 시스템이나 계산 구조(예: 이자율 커브, 가격 모델 등)를 초기화하기 위한,
// 상속 가능한 도우미 역할을 하는 기반 클래스
/*! 이 클래스는 수익률 곡선(또는 기간 구조)을 부트스트랩하는 데 사용되는 금융상품들에 대한 추상화를 제공.
    부트스트래핑에 사용되는 헬퍼 클래스는,
    해당 금융상품(예: 예금, 스왑 등)의 실제 인스턴스를 포함하도록 구성하는 것이 바람직.
    그래야 부트스트래핑 과정에서 사용되는 알고리즘과, 
    나중에 같은 상품을 개별적으로 가격 계산할 때 사용하는 알고리즘 간의 일관성을 유지할 수 있다.
    다만 현재 제공되는 BootstrapHelper 클래스들에서는 이 원칙이 완전히 강제되지는 않고 있다.
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