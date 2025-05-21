#include "pch.h"
#include "BusinessDayConvention.h"

std::ostream& operator<<(std::ostream& out, BusinessDayConvention b) 
{
    switch (b) {
    case kFollowing:
        return out << "Following";
    case kModifiedFollowing:
        return out << "Modified Following";
    case kHalfMonthModifiedFollowing:
        return out << "Half-Month Modified Following";
    case kPreceding:
        return out << "Preceding";
    case kModifiedPreceding:
        return out << "Modified Preceding";
    case kUnadjusted:
        return out << "Unadjusted";
    case kNearest:
        return out << "Nearest";
    default:
        SF_FAIL("unknown BusinessDayConvention (" << Integer(b) << ")");
    }
}