#pragma once
#include <vector>

#include "Allocator.h"

template<typename T>
using Vector = std::vector<T, StlAllocator<T>>;