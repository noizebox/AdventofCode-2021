#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>
#include <cmath>

#include "data/data_day_7.h"

constexpr int SEARCH_RANGE = 2;

void solve_day_7_pt1()
{
    /* Assume that the optimal position is roughly equal to the median position
     * of the crabs. Solve by calculating the total fuel for a small range around
     * the median and find the lowest value in this range  */

    std::vector<int> in(DATA_7_12.begin(), DATA_7_12.end());
    std::sort(in.begin(), in.end());
    int median = in[in.size() / 2];

    std::vector<int> total_fuel;
    for (int i = median - SEARCH_RANGE; i < median + SEARCH_RANGE; ++i)
    {
        total_fuel.push_back(std::accumulate(in.begin(), in.end(), 0, [&](auto a, auto b) {return a + std::abs(i - b);}));
    }

    std::cout << "Part 1, minimum total fuel: " << *std::min_element(total_fuel.begin(), total_fuel.end()) << std::endl;
}

// Recursive factorial implementation
int64_t factorial(int64_t i)
{
    return i <= 1? 1 : i + factorial(i - 1);
}

void solve_day_7_pt2()
{
    /* Same method as for part 1, but this time assume the mean is a good starting
     * point for finding the minimum. */

    std::vector<int> in(DATA_7_12.begin(), DATA_7_12.end());
    int mean = std::accumulate(in.begin(), in.end(), 0) / in.size();

    std::vector<int> total_fuel;
    for (int i = mean - SEARCH_RANGE; i < mean + SEARCH_RANGE; ++i)
    {
        total_fuel.push_back(std::accumulate(in.begin(), in.end(), 0, [&](auto a, auto b) {return a + factorial(std::abs(i - b));}));
    }

    std::cout << "Part 2, minimum total fuel: " << *std::min_element(total_fuel.begin(), total_fuel.end()) << std::endl;
}

int main()
{
    solve_day_7_pt1();
    solve_day_7_pt2();
    return 0;
}