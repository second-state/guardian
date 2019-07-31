#pragma once

#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

template<typename T, typename U> bool contain(T &&container, U &&val)
{
    return std::find(begin(container), end(container), val) != end(container);
}
