#include "pch.h"
#include "Frequency.h"

std::ostream& operator<<(std::ostream& out, Frequency f) 
{
    switch (f) {
    case kNoFrequency:
        return out << "No-Frequency";
    case kOnce:
        return out << "Once";
    case kAnnual:
        return out << "Annual";
    case kSemiannual:
        return out << "Semiannual";
    case kEveryFourthMonth:
        return out << "Every-Fourth-Month";
    case kQuarterly:
        return out << "Quarterly";
    case kBimonthly:
        return out << "Bimonthly";
    case kMonthly:
        return out << "Monthly";
    case kEveryFourthWeek:
        return out << "Every-fourth-week";
    case kBiweekly:
        return out << "Biweekly";
    case kWeekly:
        return out << "Weekly";
    case kDaily:
        return out << "Daily";
    case kOtherFrequency:
        return out << "Unknown frequency";
    default:
        SF_FAIL("unknown frequency (" << Integer(f) << ")");
    }
}