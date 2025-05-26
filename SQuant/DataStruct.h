#pragma once

struct PriceTick {
    std::string symbol;
    double open;
    double high;
    double low;
    double close;
    double volume;
    std::chrono::system_clock::time_point time;
};