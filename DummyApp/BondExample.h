#pragma once
#include "../FinanceCore/time/Calendar.h"
#include "../FinanceCore/time/calendars/TARGET.h"
#include "CorePch.h"
#include <time/TimeUnit.h>
#include <vector>

void BondExample()
{
    /////////////////// 1. 환경 설정 및 평가일 지정
    // TARGET(): 유로존 거래 달력.
    Calendar calendar = TARGET();

    // settlementDate: 채권 결제일.
    Date settlementDate(18, kSeptember, 2008);

    Integer settlementDays = 3;

    // evaluationDate: 오늘 날짜 기준 평가일로 설정.
    Date todaysDate = calendar.Advance(settlementDate, -settlementDays, TimeUnit::kDays);
    // Settings::instance().evaluationDate() = todaysDate;

    std::cout << "Today: " << todaysDate.GetWeekday() << ", " << todaysDate << std::endl;
    std::cout << "Settlement date: " << settlementDate.GetWeekday() << ", " << settlementDate << std::endl;
    /////////////////// 1. 환경 설정 및 평가일 지정



    /////////////////// 2. Discounting Bond Curve 구축 (할인율 곡선)
    Real redemption = 100.0;

    // 5개의 정부채 (FixedRateBond) 데이터
    const Size numberOfBonds = 5;

    // 발행일(issueDate), 만기일(maturity), 쿠폰(coupon), 시장 가격(marketQuotes)을 기반
    Date issueDates[] = {
        Date(15, kMarch, 2005),
        Date(15, kJune, 2005),
        Date(30, kJune, 2006),
        Date(15, kNovember, 2002),
        Date(15, kMay, 1987)
    };

    Date maturities[] = {
        Date(31, kAugust, 2010),
        Date(31, kAugust, 2011),
        Date(31, kAugust, 2013),
        Date(15, kAugust, 2018),
        Date(15, kMay, 2038)
    };

    Real couponRates[] = {
        0.02375,
        0.04625,
        0.03125,
        0.04000,
        0.04500
    };

    Real marketQuotes[] = {
        100.390625,
        106.21875,
        100.59375,
        101.6875,
        102.140625
    };

    std::vector<std::shared_ptr<SimpleQuote>> quote;
    for (Real marketQuote : marketQuotes) {
        quote.push_back(MakeShared<SimpleQuote>(marketQuote));
    }
    /*

    RelinkableHandle<Quote> quoteHandle[numberOfBonds];
    for (Size i = 0; i < numberOfBonds; i++) {
        quoteHandle[i].linkTo(quote[i]);
    }

    // FixedRateBondHelper를 사용해 RateHelper 벡터 생성
    // 이 벡터로 PiecewiseYieldCurve<Discount, LogLinear> 구조의 할인 곡선 생성
    // auto bondDiscountingTermStructure = ext::make_shared<PiecewiseYieldCurve<Discount,LogLinear>>(todaysDate, bondHelpers, termStructureDayCounter);

    std::vector<ext::shared_ptr<RateHelper>> bondHelpers;

    for (Size i = 0; i < numberOfBonds; i++) {

        Schedule schedule(issueDates[i], maturities[i], Period(Semiannual), calendar,
            Unadjusted, Unadjusted, DateGeneration::Backward, false);

        auto bondHelper = ext::make_shared<FixedRateBondHelper>(
            quoteHandle[i],
            settlementDays,
            100.0,
            schedule,
            std::vector<Rate>(1, couponRates[i]),
            ActualActual(ActualActual::Bond),
            Unadjusted,
            redemption,
            issueDates[i]);

        // the above could also be done by creating a
        // FixedRateBond instance and writing:
        //
        // auto bondHelper = ext::make_shared<BondHelper>(quoteHandle[i], bond);
        //
        // This would also work for bonds that still don't have a
        // specialized helper, such as floating-rate bonds.

        bondHelpers.push_back(bondHelper);
    }

    // The term structure uses its day counter internally to
    // convert between dates and times; it's not required to equal
    // the day counter of the bonds.  In fact, a regular day
    // counter is probably more appropriate.

    DayCounter termStructureDayCounter = Actual365Fixed();

    // The reference date of the term structure can be the
    // settlement date of the bonds (since, during pricing, it
    // won't be required to discount behind that date) but it can
    // also be today's date.  This allows one to calculate both
    // the price of the bond (based on the settlement date) and
    // the NPV, that is, the value as of today's date of holding
    // the bond and receiving its payments.

    auto bondDiscountingTermStructure = ext::make_shared<PiecewiseYieldCurve<Discount, LogLinear>>(
        todaysDate, bondHelpers, termStructureDayCounter);

    
    ////////////////////// BUILDING THE EURIBOR FORECASTING CURVE //////////////////////
    ////////////////////// 3. Euribor Forecast Curve 구축 //////////////////////
    // 6개월물 예금 + 여러 개의 고정-변동 스왑 금리 (SwapRate)
    // 각각 DepositRateHelper, SwapRateHelper로 생성 후 벡터에 저장
    // 역시 PiecewiseYieldCurve<Discount, LogLinear>로 예측 곡선 생성
    // auto depoSwapTermStructure = ext::make_shared<PiecewiseYieldCurve<Discount, LogLinear>>(spotDate, depoSwapInstruments, termStructureDayCounter);

    // 6m deposit
    Rate d6mQuote = 0.03385;
    // swaps, fixed vs 6m
    Rate s2yQuote = 0.0295;
    Rate s3yQuote = 0.0323;
    Rate s5yQuote = 0.0359;
    Rate s10yQuote = 0.0412;
    Rate s15yQuote = 0.0433;

    auto d6mRate = ext::make_shared<SimpleQuote>(d6mQuote);
    auto s2yRate = ext::make_shared<SimpleQuote>(s2yQuote);
    auto s3yRate = ext::make_shared<SimpleQuote>(s3yQuote);
    auto s5yRate = ext::make_shared<SimpleQuote>(s5yQuote);
    auto s10yRate = ext::make_shared<SimpleQuote>(s10yQuote);
    auto s15yRate = ext::make_shared<SimpleQuote>(s15yQuote);

    // setup depo
    DayCounter depositDayCounter = Actual360();
    Natural fixingDays = 2;

    auto d6m = ext::make_shared<DepositRateHelper>(
        Handle<Quote>(d6mRate),
        6 * Months, fixingDays,
        calendar, ModifiedFollowing,
        true, depositDayCounter);

    // setup swaps
    auto swFixedLegFrequency = Annual;
    auto swFixedLegConvention = Unadjusted;
    auto swFixedLegDayCounter = Thirty360(Thirty360::European);
    auto swFloatingLegIndex = ext::make_shared<Euribor6M>();

    auto s2y = ext::make_shared<SwapRateHelper>(
        Handle<Quote>(s2yRate), 2 * Years,
        calendar, swFixedLegFrequency,
        swFixedLegConvention, swFixedLegDayCounter,
        swFloatingLegIndex);
    auto s3y = ext::make_shared<SwapRateHelper>(
        Handle<Quote>(s3yRate), 3 * Years,
        calendar, swFixedLegFrequency,
        swFixedLegConvention, swFixedLegDayCounter,
        swFloatingLegIndex);
    auto s5y = ext::make_shared<SwapRateHelper>(
        Handle<Quote>(s5yRate), 5 * Years,
        calendar, swFixedLegFrequency,
        swFixedLegConvention, swFixedLegDayCounter,
        swFloatingLegIndex);
    auto s10y = ext::make_shared<SwapRateHelper>(
        Handle<Quote>(s10yRate), 10 * Years,
        calendar, swFixedLegFrequency,
        swFixedLegConvention, swFixedLegDayCounter,
        swFloatingLegIndex);
    auto s15y = ext::make_shared<SwapRateHelper>(
        Handle<Quote>(s15yRate), 15 * Years,
        calendar, swFixedLegFrequency,
        swFixedLegConvention, swFixedLegDayCounter,
        swFloatingLegIndex);

    std::vector<ext::shared_ptr<RateHelper>> depoSwapInstruments;
    depoSwapInstruments.push_back(d6m);
    depoSwapInstruments.push_back(s2y);
    depoSwapInstruments.push_back(s3y);
    depoSwapInstruments.push_back(s5y);
    depoSwapInstruments.push_back(s10y);
    depoSwapInstruments.push_back(s15y);

    // The start of the curve can be today's date or spot, depending on your preferences.
    // Here we're picking spot (mostly because we picked today's date for the bond curve).
    Date spotDate = calendar.advance(todaysDate, fixingDays, Days);
    auto depoSwapTermStructure = ext::make_shared<PiecewiseYieldCurve<Discount, LogLinear>>(
        spotDate, depoSwapInstruments,
        termStructureDayCounter);

    //////////////////////////////////////// PRICING ////////////////////////////////////////
    // 4. 세 가지 종류의 채권 생성 및 평가
    // 1. Zero-Coupon Bond (무이표 채권)
    // 발행일: 2003-08-15 / 만기일: 2013-08-15
    // 가격: 116.92

    // 2. Fixed-Rate Bond (고정금리 채권)
    // 쿠폰: 4.5%
    // 발행일: 2007-05-15 / 만기일: 2017-05-15

    // 3. Floating-Rate Bond (변동금리 채권)
    // 기준: Euribor 6M + 0.1%
    // 발행일: 2005-10-21 / 만기일: 2010-10-21
    // Gearings = 1.0, Spread = 0.001 (10bp)
    // 모두 DiscountingBondEngine을 사용해 할인 커브를 통해 가격 평가.

    RelinkableHandle<YieldTermStructure> discountingTermStructure;
    RelinkableHandle<YieldTermStructure> forecastingTermStructure;

    // bonds to be priced

    // Common data
    Real faceAmount = 100;

    // Pricing engine
    auto bondEngine = ext::make_shared<DiscountingBondEngine>(discountingTermStructure);

    // Zero coupon bond
    ZeroCouponBond zeroCouponBond(
        settlementDays,
        TARGET(),
        faceAmount,
        Date(15, August, 2013),
        Following,
        Real(116.92),
        Date(15, August, 2003));

    zeroCouponBond.setPricingEngine(bondEngine);

    // Fixed 4.5% bond
    Schedule fixedBondSchedule(
        Date(15, May, 2007), Date(15, May, 2017), Period(Annual),
        TARGET(), Unadjusted, Unadjusted, DateGeneration::Backward, false);

    FixedRateBond fixedRateBond(
        settlementDays,
        faceAmount,
        fixedBondSchedule,
        std::vector<Rate>(1, 0.045),
        ActualActual(ActualActual::Bond),
        ModifiedFollowing,
        100.0, Date(15, May, 2007));

    fixedRateBond.setPricingEngine(bondEngine);

    // Floating rate bond (6M Euribor + 0.1%)

    const auto euribor6m = ext::make_shared<Euribor>(Period(6, Months), forecastingTermStructure);
    euribor6m->addFixing(Date(18, October, 2007), 0.026);
    euribor6m->addFixing(Date(17, April, 2008), 0.028);

    Schedule floatingBondSchedule(
        Date(21, October, 2005), Date(21, October, 2010), Period(Semiannual),
        TARGET(), Unadjusted, Unadjusted, DateGeneration::Backward, true);

    FloatingRateBond floatingRateBond(
        settlementDays,
        faceAmount,
        floatingBondSchedule,
        euribor6m,
        Actual360(),
        ModifiedFollowing,
        Natural(2),
        // Gearings
        std::vector<Real>(1, 1.0),
        // Spreads
        std::vector<Rate>(1, 0.001),
        // Caps
        std::vector<Rate>(),
        // Floors
        std::vector<Rate>(),
        // Fixing in arrears
        false,
        Real(100.0),
        Date(21, October, 2005));

    floatingRateBond.setPricingEngine(bondEngine);

    // Coupon pricers
    auto pricer = ext::make_shared<BlackIborCouponPricer>();

    // optionLet volatilities
    Volatility volatility = 0.0;
    Handle<OptionletVolatilityStructure> vol;
    vol = Handle<OptionletVolatilityStructure>(
        ext::make_shared<ConstantOptionletVolatility>(
            settlementDays,
            calendar,
            ModifiedFollowing,
            volatility,
            Actual365Fixed()));

    pricer->setCapletVolatility(vol);
    setCouponPricer(floatingRateBond.cashflows(), pricer);

    forecastingTermStructure.linkTo(depoSwapTermStructure);
    discountingTermStructure.linkTo(bondDiscountingTermStructure);

    std::cout << std::endl;

    //////////////////////////// 5. 채권별 가격 및 수익률 출력 ////////////////////////////
    // NPV: 현재 가치
    // Clean Price: 이자 제외 가격
    // Dirty Price: 이자 포함 가격
    // Yield: 수익률
    // Accrued Coupon: 발생한 이자


    // write column headings
    Size widths[] = { 18, 10, 10, 10 };

    std::cout << std::setw(widths[0]) << "                 "
        << std::setw(widths[1]) << "ZC"
        << std::setw(widths[2]) << "Fixed"
        << std::setw(widths[3]) << "Floating"
        << std::endl;

    Size width = widths[0] + widths[1] + widths[2] + widths[3];
    std::string rule(width, '-');

    std::cout << rule << std::endl;

    std::cout << std::fixed;
    std::cout << std::setprecision(2);

    std::cout << std::setw(widths[0]) << "Net present value"
        << std::setw(widths[1]) << zeroCouponBond.NPV()
        << std::setw(widths[2]) << fixedRateBond.NPV()
        << std::setw(widths[3]) << floatingRateBond.NPV()
        << std::endl;

    std::cout << std::setw(widths[0]) << "Clean price"
        << std::setw(widths[1]) << zeroCouponBond.cleanPrice()
        << std::setw(widths[2]) << fixedRateBond.cleanPrice()
        << std::setw(widths[3]) << floatingRateBond.cleanPrice()
        << std::endl;

    std::cout << std::setw(widths[0]) << "Dirty price"
        << std::setw(widths[1]) << zeroCouponBond.dirtyPrice()
        << std::setw(widths[2]) << fixedRateBond.dirtyPrice()
        << std::setw(widths[3]) << floatingRateBond.dirtyPrice()
        << std::endl;

    std::cout << std::setw(widths[0]) << "Accrued coupon"
        << std::setw(widths[1]) << zeroCouponBond.accruedAmount()
        << std::setw(widths[2]) << fixedRateBond.accruedAmount()
        << std::setw(widths[3]) << floatingRateBond.accruedAmount()
        << std::endl;

    std::cout << std::setw(widths[0]) << "Previous coupon"
        << std::setw(widths[1]) << "N/A" // zeroCouponBond
        << std::setw(widths[2]) << io::rate(fixedRateBond.previousCouponRate())
        << std::setw(widths[3]) << io::rate(floatingRateBond.previousCouponRate())
        << std::endl;

    std::cout << std::setw(widths[0]) << "Next coupon"
        << std::setw(widths[1]) << "N/A" // zeroCouponBond
        << std::setw(widths[2]) << io::rate(fixedRateBond.nextCouponRate())
        << std::setw(widths[3]) << io::rate(floatingRateBond.nextCouponRate())
        << std::endl;

    std::cout << std::setw(widths[0]) << "Yield"
        << std::setw(widths[1])
        << io::rate(zeroCouponBond.yield(Actual360(), Compounded, Annual))
        << std::setw(widths[2])
        << io::rate(fixedRateBond.yield(Actual360(), Compounded, Annual))
        << std::setw(widths[3])
        << io::rate(floatingRateBond.yield(Actual360(), Compounded, Annual))
        << std::endl;

    std::cout << std::endl;

    // Other computations
    std::cout << "Sample indirect computations (for the floating rate bond): " << std::endl;
    std::cout << rule << std::endl;

    std::cout << "Yield to Clean Price: "
        << floatingRateBond.cleanPrice(floatingRateBond.yield(Actual360(), Compounded, Annual), Actual360(), Compounded, Annual, settlementDate) << std::endl;

    std::cout << "Clean Price to Yield: "
        << io::rate(floatingRateBond.yield({ floatingRateBond.cleanPrice(), Bond::Price::Clean },
            Actual360(), Compounded, Annual, settlementDate))
        << std::endl
        << std::endl;

    return 0;
    */
}