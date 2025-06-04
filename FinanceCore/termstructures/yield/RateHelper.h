#pragma once
#include "termstructures/BootstrapHelper.h"

using RateHelper = BootstrapHelper<YieldTermStructure>;
using RelativeDateRateHelper = RelativeDateBootstrapHelper<YieldTermStructure>;

class RateHelper
{
};

