#pragma once
#include "../Calendar.h"

class TARGET : public Calendar
{
private:
    class Impl final : public Calendar::WesternImpl {
    public:
        std::string GetName() const override { return "TARGET"; }
        bool IsBusinessDay(const Date&) const override;
    };
public:
    TARGET();
};