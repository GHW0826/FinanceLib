#pragma once

template <class Type>
class Null;

// T Ÿ�� ��ó�� �ڵ� ��ȯ�Ǿ� ����� �� �ְ�, �� ��ȯ��
template <typename T>
class Null {
public:
    Null() = default;
    operator T() const {
        if constexpr (std::is_floating_point_v<T>) {
            // T�� �Ǽ��� (float, double, long double)�̸� ���� ū float ���� ����.
            return (std::numeric_limits<float>::max)();
        }
        else if constexpr (std::is_integral_v<T>) {
            // T�� ������ (int, long, short, ...)�̸� int�� �ִ밪(���� 2, 147, 483, 647) �� ����
            return (std::numeric_limits<int>::max)();
        }
        else {
            // �⺻ �����ڸ� ȣ���� �� �Ϲ������� zero �Ǵ� default ��
            return T();
        }
    }
};