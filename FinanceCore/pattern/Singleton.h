#pragma once

template <class T, class Global = std::integral_constant<bool, false>>
class Singleton 
{
public:
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;
    ~Singleton() = default;
protected:
    Singleton() = default;
public:
    static T& Instance();
};

template <class T, class Global>
T& Singleton<T, Global>::Instance()
{
    static T instance;
    return instance;
}