#include <iostream>
#include <algorithm>
#include <vector>

#include "data/data_day_6.h"

constexpr int FISH_START_TIME = 8;

int growth_function(const int& age)
{
    return age > 0? age - 1 : 6;
}

// Recursively count the number of fish 1 fish spawns after n days
int64_t fish_after(int days)
{
    int64_t fish = 1;
    days -= 9;
    while (days >= 0)
    {
        fish += fish_after(days);
        days -= 7;
    }
    return fish;
}

void solve_day_6(int days)
{
    std::vector<int> fish(DATA_6_12.begin(), DATA_6_12.end());

    for (int i = 0; i < days; ++i)
    {
        int n_new_fish = std::count(fish.begin(), fish.end(), 0);
        std::transform(fish.begin(), fish.end(), fish.begin(), growth_function);
        fish.insert(fish.end(), n_new_fish, FISH_START_TIME);
    }

    std::cout << "Done, size after " << days << " iterations: " << fish.size() << std::endl;
}

void solve_day_6_optimised(int days)
{
    /* Recursively calculating the number of fish a single lantern fish will spawn after
     * 256 days is manageable, but still takes a couple of seconds. So we don't want to
     * do that more than once per starting value.
     * So start by counting the occurrences of every element i the data */

    int64_t fish = 0;

    for (int i = 0; i <= FISH_START_TIME; ++i)
    {
        int count = std::count(DATA_6_12.begin(), DATA_6_12.end(), i);
        if (count > 0)
        {
            fish += count * fish_after(days + FISH_START_TIME - i);
        }
    }

    std::cout << "Total: " << fish << " after " << days << " days" << std::endl;
}

int main()
{
    // Part 1
    solve_day_6(80);
    // Part 2
    solve_day_6_optimised(256);
    return 0;
}