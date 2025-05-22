#pragma once

enum Frequency {
    kNoFrequency = -1,     //!< null frequency
    kOnce = 0,             //!< only once, e.g., a zero-coupon
    kAnnual = 1,           //!< once a year
    kSemiannual = 2,       //!< twice a year
    kEveryFourthMonth = 3, //!< every fourth month
    kQuarterly = 4,        //!< every third month
    kBimonthly = 6,        //!< every second month
    kMonthly = 12,         //!< once a month
    kEveryFourthWeek = 13, //!< every fourth week
    kBiweekly = 26,        //!< every second week
    kWeekly = 52,          //!< once a week
    kDaily = 365,          //!< once a day
    kOtherFrequency = 999  //!< some other unknown frequency
};

/*! \relates Frequency */
std::ostream& operator<<(std::ostream& out, Frequency f);

