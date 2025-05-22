#pragma once

template <class Type>
class Null;

// T 타입 값처럼 자동 변환되어 사용할 수 있고, 그 변환값
template <typename T>
class Null {
public:
    Null() = default;
    operator T() const {
        if constexpr (std::is_floating_point_v<T>) {
            // T가 실수형 (float, double, long double)이면 아주 큰 float 값을 리턴.
            return (std::numeric_limits<float>::max)();
        }
        else if constexpr (std::is_integral_v<T>) {
            // T가 정수형 (int, long, short, ...)이면 int의 최대값(보통 2, 147, 483, 647) 을 리턴
            return (std::numeric_limits<int>::max)();
        }
        else {
            // 기본 생성자를 호출함 → 일반적으로 zero 또는 default 값
            return T();
        }
    }
};