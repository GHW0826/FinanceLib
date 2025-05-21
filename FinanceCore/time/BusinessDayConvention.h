#pragma once

enum BusinessDayConvention {
    // ISDA
    kFollowing,                   /*!< Choose the first business day after the given holiday. */
    kModifiedFollowing,           /*!< Choose the first business day after
                                      the given holiday unless it belongs
                                      to a different month, in which case
                                      choose the first business day before
                                      the holiday. */
    kPreceding,                   /*!< Choose the first business day before the given holiday. */
    
    // NON ISDA
    kModifiedPreceding,           /*!< Choose the first business day before
                                      the given holiday unless it belongs
                                      to a different month, in which case
                                      choose the first business day after
                                      the holiday. */
    kUnadjusted,                  /*!< Do not adjust. */
    kHalfMonthModifiedFollowing,  /*!< Choose the first business day after
                                      the given holiday unless that day
                                      crosses the mid-month (15th) or the
                                      end of month, in which case choose
                                      the first business day before the
                                      holiday. */
    kNearest                      /*!< Choose the nearest business day
                                      to the given holiday. If both the
                                      preceding and following business
                                      days are equally far away, default
                                      to following business day. */
};

std::ostream& operator<<(std::ostream&, BusinessDayConvention);